#include "codex.h"

void	destroy_sim(t_sim *sim)
{
	int	i;

	pthread_mutex_destroy(&sim->mutex);
	pthread_mutex_destroy(&sim->log_mutex);
	pthread_cond_destroy(&sim->cond);
	i = 0;
	while (i < sim->args->number_of_coders)
	{
		pthread_mutex_destroy(&sim->dongles[i].mutex);
		pthread_cond_destroy(&sim->dongles[i].cond);
		i++;
	}
	free(sim->coders);
	free(sim->dongles);
}
