#include "codex.h"

int is_running(t_sim *sim)
{
	int running;

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
