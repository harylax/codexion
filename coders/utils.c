#include "codex.h"

int	is_running(t_sim *sim)
{
	int	running;

	pthread_mutex_lock(&sim->mutex);
	running = sim->running;
	pthread_mutex_unlock(&sim->mutex);
	return (running);
}

void	log_action(t_coder *coder, char *log)
{
	pthread_mutex_lock(&coder->sim->log_mutex);
	printf("%ld %d %s\n", get_timestamp_ms(coder->sim), coder->id, log);
	pthread_mutex_unlock(&coder->sim->log_mutex);
}

int	is_first_in_queue(t_coder *coder, t_dongle *dongle)
{
	if (dongle->priority.size == 0)
		return (0);
	return (coder == dongle->priority.queue[0].coder);
}

void	request_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	heap_push(coder, &dongle->priority);
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}
