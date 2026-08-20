#include "codex.h"

static void init_coders(t_sim *sim)
{
	int i = 0;
	while (i < sim->args->number_of_coders)
	{
		sim->coders[i].sim = sim;
		sim->coders[i].id = i + 1;
		sim->coders[i].compilations_done = 0;
		sim->coders[i].last_compile_start = get_timestamp_ms(sim);
		sim->coders[i].state = WORKING;
		sim->coders[i].left = &sim->dongles[i];
		sim->coders[i].right = &sim->dongles[(i + 1) % sim->args->number_of_coders];
		i++;
	}
}

static void init_dongles(t_sim *sim)
{
	int i = 0;
	while (i < sim->args->number_of_coders)
	{
		sim->dongles[i].sim = sim;
		sim->dongles[i].id = i + 1;
		sim->dongles[i].available = 1;
		sim->dongles[i].hot = 0;
		sim->dongles[i].users[0] = &sim->coders[i];
		sim->dongles[i].users[1] = &sim->coders[(i - 1 + sim->args->number_of_coders) % sim->args->number_of_coders];
		sim->dongles[i].priority.size = 0;
		i++;
	}
}

int init_sim(t_sim *sim, t_arg *arg)
{
	sim->running = 1;
	sim->args = arg;
	gettimeofday(&sim->start_time, NULL);
	sim->coders = malloc(arg->number_of_coders * sizeof(t_coder));
	sim->dongles = malloc(arg->number_of_coders * sizeof(t_dongle));
	if (!sim->coders || !sim->dongles)
	{
		free(sim->coders);
		free(sim->dongles);
		return (0);
	}
	init_coders(sim);
	init_dongles(sim);
	pthread_mutex_init(&sim->mutex, NULL);
	pthread_mutex_init(&sim->log_mutex, NULL);
	pthread_cond_init(&sim->cond, NULL);
	return (1);
}

void destroy_sim(t_sim *sim)
{
	pthread_mutex_destroy(&sim->mutex);
	pthread_mutex_destroy(&sim->log_mutex);
	pthread_cond_destroy(&sim->cond);
	free(sim->coders);
	free(sim->dongles);
}