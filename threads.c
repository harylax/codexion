#include "codex.h"

int	start_threads(t_sim *sim)
{
	int	i;
	int	n;

	n = sim->args->number_of_coders;
	if (pthread_create(&sim->monitor_thread, NULL, monitor_routine, sim))
		return (0);
	i = 0;
	while (i < n)
	{
		if (pthread_create(&sim->coders[i].thread, NULL,
				coder_routine, &sim->coders[i]))
			return (0);
		if (pthread_create(&sim->dongles[i].thread, NULL,
				dongle_routine, &sim->dongles[i]))
			return (0);
		i++;
	}
	return (1);
}

void	join_threads(t_sim *sim)
{
	int	i;
	int	n;

	n = sim->args->number_of_coders;
	i = 0;
	while (i < n)
	{
		pthread_join(sim->coders[i].thread, NULL);
		pthread_join(sim->dongles[i].thread, NULL);
		i++;
	}
	pthread_join(sim->monitor_thread, NULL);
}
