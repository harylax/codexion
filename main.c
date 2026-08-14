#include "codex.h"

void *coder_routine(void *arg)
{
	t_coder *coder = (t_coder *)arg;
	int count_to_burnout = 0;
	while (1)
	{
		pthread_mutex_lock(&coder->sim->mut);
		while (coder->left->available != 1)
			pthread_cond_wait(&coder->sim->cond, &coder->sim->mut);
		pthread_mutex_unlock(&coder->sim->mut);

		pthread_mutex_lock(&coder->sim->mut);
		coder->left->available = 0;
		pthread_mutex_unlock(&coder->sim->mut);

		pthread_mutex_lock(&coder->sim->mut);
		while (coder->right->available != 1)
		pthread_cond_wait(&coder->sim->cond, &coder->sim->mut);
		pthread_mutex_unlock(&coder->sim->mut);
		
		pthread_mutex_lock(&coder->sim->mut);
		coder->right->available = 0;
		pthread_mutex_unlock(&coder->sim->mut);
		
		usleep(coder->sim->args->time_to_compile);
		coder->compilations_done += 1;
		if (coder->compilations_done >= coder->sim->args->number_of_compiles_required)
		{
			coder->state = DONE;
			break;
		}
		pthread_mutex_lock(&coder->sim->mut);
		coder->left->hot = 1;
		coder->right->hot = 1;
		pthread_mutex_unlock(&coder->sim->mut);

		pthread_cond_broadcast(&coder->sim->cond);

		usleep(coder->sim->args->time_to_debug);
		usleep(coder->sim->args->time_to_refactor);
		
		count_to_burnout += 1;
		if (count_to_burnout >= coder->sim->args->time_to_burnout)
		{
			coder->state = BURNED_OUT;
			break;
		}
	}
	return (NULL);
}

void	*dongle_routine(void *arg)
{
	t_dongle *dongle = (t_dongle *)arg;
	int stop = 0;
	int count_coders_done = 0;
	while (1)
	{
		pthread_mutex_lock(&dongle->sim->mut);
		while (dongle->hot != 1)
			pthread_cond_wait(&dongle->sim->cond, &dongle->sim->mut);
		pthread_mutex_unlock(&dongle->sim->mut);
		
		usleep(dongle->sim->args->dongle_cooldown);
		pthread_mutex_lock(&dongle->sim->mut);
		dongle->hot = 0;
		dongle->available = 1;
		pthread_mutex_unlock(&dongle->sim->mut);

		pthread_cond_broadcast(&dongle->sim->cond);
		
		int i = 0;
		while (i < dongle->sim->args->number_of_coders)
		{
			if (dongle->sim->coders[i].state == BURNED_OUT)
			{
				stop = 1;
				break;
			}
			if (dongle->sim->coders[i].state == DONE)
				count_coders_done++;
			i++;
		}
		if (stop)
			break;
		if (count_coders_done >= dongle->sim->args->number_of_coders)
			break;
	}
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
		sim.coders[i].state = WORKING;
		
		sim.dongles[i].id = i + 1;
		sim.dongles[i].available = 1;
		sim.dongles[i].hot = 0;
		
		sim.coders[i].left = &sim.dongles[i];
		sim.coders[i].right = &sim.dongles[(i + 1) % args.number_of_coders];

		i++;
	}

	return (0);
}
