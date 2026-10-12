#include "codex.h"

int	start_threads(t_sim *sim)
{
	int	i;
	int	n;

	sim->count.monitor_thread = 0;
	sim->count.coder_threads = 0;
	sim->count.dongle_threads = 0;
	n = sim->args->number_of_coders;
	if (pthread_create(&sim->monitor_thread, NULL, monitor_routine, sim))
		return (0);
	sim->count.monitor_thread = 1;
	i = 0;
	while (i < n)
	{
		if (pthread_create(&sim->coders[i].thread, NULL,
				coder_routine, &sim->coders[i]))
			return (0);
		sim->count.coder_threads++;
		if (pthread_create(&sim->dongles[i].thread, NULL,
				dongle_routine, &sim->dongles[i]))
			return (0);
		sim->count.dongle_threads++;
		i++;
	}
	return (1);
}

void	join_threads(t_sim *sim)
{
	int	i;

	if (!sim->count.monitor_thread)
		return ;
	i = 0;
	while (i < sim->count.coder_threads)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
	i = 0;
	while (i < sim->count.dongle_threads)
	{
		pthread_join(sim->dongles[i].thread, NULL);
		i++;
	}
	pthread_join(sim->monitor_thread, NULL);
}
