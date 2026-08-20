#include "codex.h"

long get_timestamp_ms(t_sim *sim)
{
	struct timeval now;
	long elapsed;

	gettimeofday(&now, NULL);
	elapsed = (now.tv_sec - sim->start_time.tv_sec) * 1000;
	elapsed += (now.tv_usec - sim->start_time.tv_usec) / 1000;
	return (elapsed);
}

void wait_timeout(t_sim *sim, long ms)
{
	struct timeval now;
	struct timespec timeout;
	long end_ms;
	
	pthread_mutex_lock(&sim->mutex);
	gettimeofday(&now, NULL);
	end_ms = (long)now.tv_sec * 1000 + now.tv_usec / 1000 + ms;
	timeout.tv_sec = end_ms / 1000;
	timeout.tv_nsec = (end_ms % 1000) * 100000;
	while (sim->running)
	{
		if (pthread_cond_timedwait(&sim->cond, &sim->mutex, &timeout))
			break ;
	}
	pthread_mutex_unlock(&sim->mutex);
}
