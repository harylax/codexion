#include "codex.h"

static int is_burnout(t_coder *coder)
{
	if (coder->state != WORKING)
		return (0);
	return (get_timestamp_ms(coder->sim) >= coder->last_compile_start + coder->sim->args->time_to_burnout);
}

static int check_burnout(t_sim *sim)
{
	int i = 0;
	int burnout;

	while (i < sim->args->number_of_coders)
	{
		pthread_mutex_lock(&sim->mutex);
		burnout = is_burnout(&sim->coders[i]);
		pthread_mutex_unlock(&sim->mutex);
		if (burnout)
			return (sim->coders[i].id);
		i++;
	}
	return (0);
}

static int	all_done(t_sim *sim)
{
	int i;
	int out;

	i= 0;
	out = 1;
	pthread_mutex_lock(&sim->mutex);
	while (i < sim->args->number_of_coders)
	{
		if (sim->coders[i].state == WORKING)
		{
			out = 0;
			break ;
		}
		i++;
	}
	pthread_mutex_unlock(&sim->mutex);
	return (out);
}

static void	stop_sim(t_sim *sim, t_end end, int burnout_id)
{
	if (end == FAIL)
	{
		pthread_mutex_lock(&sim->mutex);
		sim->running = 0;
		sim->coders[burnout_id - 1].state = BURNED_OUT;
		log_action(&sim->coders[burnout_id - 1], "burned out");
		pthread_cond_broadcast(&sim->cond);
		pthread_mutex_unlock(&sim->mutex);
		return ;
	}
	pthread_mutex_lock(&sim->mutex);
	sim->running = 0;
	pthread_cond_broadcast(&sim->cond);
	pthread_mutex_unlock(&sim->mutex);
}

void	*monitor_routine(void *arg)
{
	t_sim *sim;
	int burnout_id;
	
	sim = (t_sim *)arg;
	while (is_running(sim))
	{
		burnout_id = check_burnout(sim);
		if (burnout_id)
		{
			stop_sim(sim, FAIL, burnout_id);
			break ;
		}
		if (all_done(sim))
		{
			stop_sim(sim, SUCCESS, 0);
			break ;
		}
		usleep(1000);
	}
	return (NULL);
}
