#include "codex.h"


void *coder_routine(void *arg)
{
	t_coder *coder = (t_coder *)arg;
	t_dongle *first, *second;

	if (coder->id % 2)
	{
		first = coder->left;
		second = coder->right;
	}
	else
	{
		first = coder->right;
		second =coder->left;
	}
	while (1)
	{
		pthread_mutex_lock(&coder->sim->mutex);
		while (first->available == 0)
			pthread_cond_wait(&coder->sim->cond, &coder->sim->mutex);
		first->available = 0;
		printf("coder %d has taken dongle %d\n", coder->id, first->id);
		while (second->available == 0)
			pthread_cond_wait(&coder->sim->cond, &coder->sim->mutex);
		second->available = 0;		
		printf("coder %d has taken dongle %d\n", coder->id, second->id);
		pthread_mutex_unlock(&coder->sim->mutex);
		
		
		printf("coder %d is compiling\n", coder->id);
		usleep(coder->sim->args->time_to_compile);
		coder->compilations_done++;
		// restart time_to_burnout counter
		pthread_mutex_lock(&coder->sim->mutex);
		first->hot = 1;
		second->hot = 1;			
		pthread_mutex_unlock(&coder->sim->mutex);
		pthread_cond_broadcast(&coder->sim->cond);			
		if (coder->compilations_done >= coder->sim->args->number_of_compiles_required)
		{
			// stop time_to_burnout counter
			coder->state = DONE;
			printf("coder %d is done\n", coder->id);
			break;
		}
		
		printf("coder %d is debugging\n", coder->id);
		usleep(coder->sim->args->time_to_debug);
		printf("coder %d is refactoring\n", coder->id);
		usleep(coder->sim->args->time_to_refactor);
		// need to check burnout, where, when
	}
	return (NULL);
}

void	*dongle_routine(void *arg)
{
	t_dongle *dongle = (t_dongle *)arg;
	while (1)
	{
		pthread_mutex_lock(&dongle->sim->mutex);
		while (dongle->hot == 0 && (dongle->users[0]->state == WORKING || dongle->users[1]->state == WORKING))
		{
			pthread_cond_wait(&dongle->sim->cond, &dongle->sim->mutex);
		}
		pthread_mutex_unlock(&dongle->sim->mutex);
		usleep(dongle->sim->args->dongle_cooldown);

		pthread_mutex_lock(&dongle->sim->mutex);
		dongle->hot = 0;
		dongle->available = 1;
		pthread_mutex_unlock(&dongle->sim->mutex);
		pthread_cond_broadcast(&dongle->sim->cond);

		if (dongle->users[0]->state == DONE && dongle->users[1]->state == DONE)
		{
			printf("dongle %d is out of use\n", dongle->id);
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
	sim.args = &args;
	sim.coders = malloc(args.number_of_coders * sizeof(t_coder));
	sim.dongles = malloc(args.number_of_coders * sizeof(t_dongle));

	int i = 0;
	while (i < args.number_of_coders)
	{
		sim.coders[i].sim = &sim;
		sim.coders[i].id = i + 1;
		sim.coders[i].compilations_done = 0;
		sim.coders[i].state = WORKING;
		
		sim.dongles[i].sim = &sim;
		sim.dongles[i].id = i + 1;
		sim.dongles[i].available = 1;
		sim.dongles[i].hot = 0;
		sim.dongles[i].users[0] = &sim.coders[i];
		sim.dongles[i].users[1] = &sim.coders[(i + 1) % args.number_of_coders];
		
		sim.coders[i].left = &sim.dongles[i];
		sim.coders[i].right = &sim.dongles[(i + 1) % args.number_of_coders];
		
		i++;
	}

	pthread_mutex_init(&sim.mutex, NULL);
	pthread_cond_init(&sim.cond, NULL);

	i = 0;
	while (i < args.number_of_coders)
	{
		pthread_create(&sim.coders[i].thread, NULL, coder_routine, &sim.coders[i]);
		pthread_create(&sim.dongles[i].thread, NULL, dongle_routine, &sim.dongles[i]);
		i++;
	}

	i = 0;
	while (i < args.number_of_coders)
	{
		pthread_join(sim.coders[i].thread, NULL);
		pthread_join(sim.dongles[i].thread, NULL);
		i++;
	}

	pthread_mutex_destroy(&sim.mutex);
	pthread_cond_destroy(&sim.cond);

	free(sim.coders);
	free(sim.dongles);
	return (0);
}
