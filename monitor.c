#include "codex.h"

static int is_burnout(t_coder *coder)
{
	long deadline;
	long now;

	if (coder->state != WORKING)
		return (0);
	now = get_timestamp_ms(coder->sim);
	deadline = coder->last_compile_start + coder->sim->args->time_to_burnout;
	return (now >= deadline);
}

static int check_burnout(t_sim *sim)
{
	int i = 0;
	int id;

	while (i < sim->args->number_of_coders)
	{
		pthread_mutex_lock(&sim->mutex);
		if (is_burnout(&sim->coders[i]))
		{
			id = sim->coders[i].id;
			pthread_mutex_unlock(&sim->mutex);
			return (id);
		}
		pthread_mutex_unlock(&sim->mutex);
		i++;
	}
	return (0);
}

static int	all_done(t_sim *sim)
{
	int i;

	i= 0;
	pthread_mutex_lock(&sim->mutex);
	while (i < sim->args->number_of_coders)
	{
		if (sim->coders[i].state == WORKING)
		{
			pthread_mutex_unlock(&sim->mutex);
			return (0);
		}
		i++;
	}
	pthread_mutex_unlock(&sim->mutex);
	return (1);
}

static void	stop_sim(t_sim *sim, t_end end, int burnout_id)
{
	pthread_mutex_lock(&sim->mutex);
	sim->running = 0;
	if (end == FAIL)
	{
		sim->coders[burnout_id - 1].state = BURNED_OUT;
		log_action(&sim->coders[burnout_id - 1], "burned out");
	}
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
