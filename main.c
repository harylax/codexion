#include "codex.h"

void *coder_routine(void *arg)
{
	t_coder *coder = (t_coder *)arg;
	int count_to_burnout = 0;
	while (1)
	{
		while (coder->left->available && coder->right->available)
		{
			pthread_cond_wait(&coder->left->cond, &coder->left->mut);
			pthread_cond_wait(&coder->right->cond, &coder->right->mut);
		}
		// COMPILATION
		// compilation lock
		pthread_mutex_lock(&coder->left->mut);
		pthread_mutex_lock(&coder->right->mut);
		coder->left->available = 0;
		printf("%d has taken a dongle\n", coder->id);
		coder->right->available = 0;
		printf("%d has taken a dongle\n", coder->id);
		printf("%d is compiling\n", coder->id);
		usleep(coder->sim->args->time_to_compile); // for the other coders
		pthread_mutex_unlock(&coder->left->mut);
		pthread_mutex_unlock(&coder->right->mut);
		// unlock
		
		coder->compilations_done += 1;
		if (coder->compilations_done >= coder->sim->args->number_of_compiles_required)
		{
			coder->state = DONE;
			printf("%d has finished", coder->id);
			break;
		}
		
		// DEBUGGING
		
		printf("%d is debugging\n", coder->id);
		usleep(coder->sim->args->time_to_debug); // for this coder
		
		// cooldown lock
		pthread_mutex_lock(&coder->left->mut);
		pthread_mutex_lock(&coder->right->mut);
		usleep(coder->sim->args->dongle_cooldown); // for all the coders
		coder->left->available = 1;
		coder->right->available = 1;
		pthread_mutex_unlock(&coder->left->mut);
		pthread_mutex_unlock(&coder->right->mut);
		// unlock
		
		// REFACTORING
		printf("%d is refactoring\n", coder->id);
		usleep(coder->sim->args->time_to_refactor); // for this coder

		count_to_burnout += 1;
		if (count_to_burnout >= coder->sim->args->time_to_burnout)
		{
			printf("%d burned out\n", coder->id);
			coder->state = BURNED_OUT;
			break;
		}
	}
	return (NULL);
}

int main(int ac, char **av)
{
	if (missing_args(ac))
		return (1);
	if (check_args(ac, av))
		return (1);
	t_arg args;
	parse_args(&args, av);

	t_sim sim;
	sim.coders = malloc(args.number_of_coders * sizeof(t_coder));
	sim.dongles = malloc(args.number_of_coders * sizeof(t_dongle));

	int i = 0;
	while (i < args.number_of_coders)
	{
		sim.coders[i].sim = &sim;
		sim.coders[i].id = i + 1;
		sim.coders[i].compilations_done = 0;
		
		sim.dongles[i].id = i + 1;
		sim.dongles[i].available = 1;
		
		sim.coders[i].left = &sim.dongles[i];
		sim.coders[i].right = &sim.dongles[(i + 1) % args.number_of_coders];

		i++;
	}

	i = 0;
	while (i < args.number_of_coders)
	{
		pthread_create(&sim.coders[i].thread, NULL, coder_routine, &sim.coders[i]);
		i++;
	}

	i = 0;
	while (i < args.number_of_coders)
	{
		pthread_join(sim.coders[i].thread, NULL);
		if (sim.coders->state == BURNED_OUT)
		{
			free(sim.coders);
			free(sim.dongles);
			return (1);
		}
		i++;
	}

	free(sim.coders);
	free(sim.dongles);
	return (0);
}


// printf("%d\n", sim.coders[6].right->id);
// printf("%d\n", sim.coders[5].left->id);

// printf("number_of_coders=%d\n", args.number_of_coders);
// printf("time_to_burnout=%d\n", args.time_to_burnout);
// printf("time_to_compile=%d\n", args.time_to_compile);
// printf("time_to_debug=%d\n", args.time_to_debug);
// printf("time_to_refactor=%d\n", args.time_to_refactor);
// printf("number_of_compiles_required=%d\n", args.number_of_compiles_required);
// printf("dongle_cooldown=%d\n", args.dongle_cooldown);
// printf("scheduler=%d\n", args.scheduler);