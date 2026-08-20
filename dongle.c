#include "codex.h"

static int	cool_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->sim->mutex);
	if (dongle->sim->args->number_of_coders == 1)
	{
		while (dongle->sim->running && dongle->hot == 0)
			pthread_cond_wait(&dongle->sim->cond, &dongle->sim->mutex);
	}
	else
	{
		while (dongle->sim->running && dongle->hot == 0
			&& (dongle->users[0]->state != DONE
			|| dongle->users[1]->state != DONE))
			pthread_cond_wait(&dongle->sim->cond, &dongle->sim->mutex);
	}
	pthread_mutex_unlock(&dongle->sim->mutex);
	if (!is_running(dongle->sim))
		return (0);
	wait_timeout(dongle->sim, dongle->sim->args->dongle_cooldown);
	pthread_mutex_lock(&dongle->sim->mutex);
	dongle->hot = 0;
	dongle->available = 1;
	pthread_mutex_unlock(&dongle->sim->mutex);
	return (1);
}

void	*dongle_routine(void *arg)
{
	t_dongle *dongle;
	int	both_done;
	
	dongle = (t_dongle *)arg;
	while (is_running(dongle->sim))
	{
		if (!cool_dongle(dongle))
			break;
		pthread_mutex_lock(&dongle->sim->mutex);
		if (dongle->sim->args->number_of_coders == 1)
			both_done = dongle->users[0]->state == DONE;
		else
			both_done = (dongle->users[0]->state == DONE
					&& dongle->users[1]->state == DONE);
		pthread_cond_broadcast(&dongle->sim->cond);
		pthread_mutex_unlock(&dongle->sim->mutex);
		if (both_done)
		{		
			pthread_mutex_lock(&dongle->sim->log_mutex);
			printf("%ld dongle %d is out of use\n", get_timestamp_ms(dongle->sim), dongle->id);
			pthread_mutex_unlock(&dongle->sim->log_mutex);
			break;
		}
	}
	return (NULL);
}
