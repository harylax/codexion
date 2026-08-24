#include "codex.h"

static void	init_coders(t_sim *sim)
{
	int	i;
	int	n_coders;

	i = 0;
	n_coders = sim->args->number_of_coders;
	while (i < n_coders)
	{
		sim->coders[i].sim = sim;
		sim->coders[i].id = i + 1;
		sim->coders[i].compilations_done = 0;
		sim->coders[i].last_compile_start = get_timestamp_ms(sim);
		sim->coders[i].state = WORKING;
		sim->coders[i].left = &sim->dongles[i];
		sim->coders[i].right = &sim->dongles[(i + 1) % n_coders];
		i++;
	}
}

static void	init_dongles(t_sim *sim)
{
	int	i;
	int	n_coders;

	i = 0;
	n_coders = sim->args->number_of_coders;
	while (i < n_coders)
	{
		sim->dongles[i].sim = sim;
		sim->dongles[i].id = i + 1;
		sim->dongles[i].available = 1;
		sim->dongles[i].hot = 0;
		sim->dongles[i].users[0] = &sim->coders[i];
		sim->dongles[i].users[1] = &sim->coders[(i - 1 + n_coders) % n_coders];
		sim->dongles[i].priority.size = 0;
		i++;
	}
}

static int	init_dongles_mutex_cond(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->args->number_of_coders)
	{
		if (pthread_mutex_init(&sim->dongles[i].mutex, NULL))
		{
			printf("Error: failed to init dongle");
			printf(" %d mutex\n", sim->dongles[i].id);
			free(sim->coders);
			free(sim->dongles);
			return (0);
		}
		if (pthread_cond_init(&sim->dongles[i].cond, NULL))
		{
			printf("Error: failed to init dongle");
			printf(" %d cond\n", sim->dongles[i].id);
			free(sim->coders);
			free(sim->dongles);
			return (0);
		}
		i++;
	}
	return (1);
}

static int	init_sim_mutex_cond(t_sim *sim)
{
	if (pthread_mutex_init(&sim->mutex, NULL))
	{
		printf("Error: failed to init mutex\n");
		free(sim->coders);
		free(sim->dongles);
		return (0);
	}
	if (pthread_mutex_init(&sim->log_mutex, NULL))
	{
		printf("Error: failed to init log_mutex\n");
		free(sim->coders);
		free(sim->dongles);
		return (0);
	}
	if (pthread_cond_init(&sim->cond, NULL))
	{
		printf("Error: failed to init condition variable\n");
		free(sim->coders);
		free(sim->dongles);
		return (0);
	}
	return (1);
}

int	init_sim(t_sim *sim, t_arg *arg)
{
	sim->running = 1;
	sim->args = arg;
	gettimeofday(&sim->start_time, NULL);
	sim->coders = malloc(arg->number_of_coders * sizeof(t_coder));
	sim->dongles = malloc(arg->number_of_coders * sizeof(t_dongle));
	if (!sim->coders || !sim->dongles)
	{
		if (sim->coders)
			free(sim->coders);
		if (sim->dongles)
			free(sim->dongles);
		printf("Error: failed to malloc\n");
		return (0);
	}
	init_coders(sim);
	init_dongles(sim);
	if (!init_sim_mutex_cond(sim))
		return (0);
	if (!init_dongles_mutex_cond(sim))
		return (0);
	return (1);
}
