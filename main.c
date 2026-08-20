#include "codex.h"

int main(int ac, char **av)
{
	t_arg	args;
	t_sim	sim;

	if (missing_args(ac) || check_args(ac, av))
		return (1);
	parse_args(&args, av);
	if (!init_sim(&sim, &args))
	{
		printf("Error: failed to malloc\n");
		return (1);
	}
	if (!start_threads(&sim))
	{
		printf("Error: failed to create threads\n");
		sim.running = 0;
		pthread_cond_broadcast(&sim.cond);
		join_threads(&sim);
		destroy_sim(&sim);
		return (1);
	}
	join_threads(&sim);
	destroy_sim(&sim);
	return (0);
}
