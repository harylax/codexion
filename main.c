#include "codex.h"

void	print_log(t_coder *coder, char *log)
{
	pthread_mutex_lock(&coder->sim->log_mutex);
	printf("%ld %d %s\n", get_timestamp_ms(coder->sim), coder->id, log);
	pthread_mutex_unlock(&coder->sim->log_mutex);
}

void	book_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&coder->sim->mutex);
	heap_push(coder, &dongle->priority);
	pthread_cond_broadcast(&coder->sim->cond);
	pthread_mutex_unlock(&coder->sim->mutex);
}

void pick_up_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&coder->sim->mutex);
	while (dongle->available == 0 || coder != dongle->priority.queue[0].coder)
		pthread_cond_wait(&coder->sim->cond, &coder->sim->mutex);
	heap_pop(&dongle->priority);
	dongle->available = 0;
	print_log(coder, "has taken dongle");
	pthread_mutex_unlock(&coder->sim->mutex);
}

void take_dongles(t_coder *coder, t_dongle *first, t_dongle *second)
{
	book_dongle(coder, first);
	pick_up_dongle(coder, first);
	book_dongle(coder, second);
	pick_up_dongle(coder, second);
}

void compile(t_coder *coder)
{
	print_log(coder, "is compiling");
	pthread_mutex_lock(&coder->sim->mutex);
	coder->last_compile_start = get_timestamp_ms(coder->sim);
	pthread_mutex_unlock(&coder->sim->mutex);
	usleep(coder->sim->args->time_to_compile * 1000);
	coder->compilations_done++;
}

void	broadcast_dongles(t_coder *coder, t_dongle *first, t_dongle *second)
{
	pthread_mutex_lock(&coder->sim->mutex);
	first->hot = 1;
	second->hot = 1;			
	if (coder->compilations_done >= coder->sim->args->number_of_compiles_required)
		coder->state = DONE;
	pthread_mutex_unlock(&coder->sim->mutex);
	pthread_cond_broadcast(&coder->sim->cond);
}

void	debug_and_refactor(t_coder *coder)
{
	print_log(coder, "is debugging");
	usleep(coder->sim->args->time_to_debug * 1000);
	print_log(coder, "is refactoring");
	usleep(coder->sim->args->time_to_refactor * 1000);
}

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
	while (coder->sim->running)
	{
		take_dongles(coder, first, second);
		
		compile(coder);

		broadcast_dongles(coder, first, second);

		if (coder->state == DONE)
		{
			print_log(coder, "is done");
			break;
		}
		
		debug_and_refactor(coder);
	}
	return (NULL);
}

void	cool_dongle(t_dongle *dongle)
{
		pthread_mutex_lock(&dongle->sim->mutex);
		while (dongle->hot == 0 && (dongle->users[0]->state != DONE || dongle->users[1]->state != DONE))
			pthread_cond_wait(&dongle->sim->cond, &dongle->sim->mutex);
		pthread_mutex_unlock(&dongle->sim->mutex);
		usleep(dongle->sim->args->dongle_cooldown * 1000);

		pthread_mutex_lock(&dongle->sim->mutex);
		dongle->hot = 0;
		dongle->available = 1;
		pthread_mutex_unlock(&dongle->sim->mutex);
}

void	*dongle_routine(void *arg)
{
	t_dongle *dongle = (t_dongle *)arg;
	int	both_done;

	while (dongle->sim->running)
	{
		cool_dongle(dongle);

		pthread_mutex_lock(&dongle->sim->mutex);
		both_done = (dongle->users[0]->state == DONE && dongle->users[1]->state == DONE);
		pthread_mutex_unlock(&dongle->sim->mutex);
		pthread_cond_broadcast(&dongle->sim->cond);

		if (both_done)
		{		
			pthread_mutex_lock(&dongle->sim->log_mutex);
			printf("%ld dongle %d is out of use\n", get_timestamp_ms(dongle->sim), dongle->id);
			pthread_mutex_unlock(&dongle->sim->log_mutex);
			break;
		}
	}
	return (NULL);
}

void *single_coder_routine(void *arg)
{
	t_coder *coder = (t_coder *)arg;
	t_dongle *dongle = coder->left;
	while (coder->sim->running)
	{
		book_dongle(coder, dongle);
		pick_up_dongle(coder, dongle);

		compile(coder);

		pthread_mutex_lock(&coder->sim->mutex);
		dongle->hot = 1;			
		if (coder->compilations_done >= coder->sim->args->number_of_compiles_required)
			coder->state = DONE;
		pthread_mutex_unlock(&coder->sim->mutex);
		pthread_cond_broadcast(&coder->sim->cond);

		if (coder->state == DONE)
		{
			print_log(coder, "is done");
			break;
		}
		debug_and_refactor(coder);
	}
	return (NULL);
}

void	*single_dongle_routine(void *arg)
{
	t_dongle *dongle = (t_dongle *)arg;
	while (dongle->sim->running)
	{
		pthread_mutex_lock(&dongle->sim->mutex);
		while (dongle->hot == 0)
			pthread_cond_wait(&dongle->sim->cond, &dongle->sim->mutex);
		pthread_mutex_unlock(&dongle->sim->mutex);
		usleep(dongle->sim->args->dongle_cooldown * 1000);

		pthread_mutex_lock(&dongle->sim->mutex);
		dongle->hot = 0;
		dongle->available = 1;
		int done = dongle->users[0]->state == DONE;
		pthread_mutex_unlock(&dongle->sim->mutex);
		pthread_cond_broadcast(&dongle->sim->cond);

		if (done)
		{		
			pthread_mutex_lock(&dongle->sim->log_mutex);
			printf("%ld dongle %d is out of use\n", get_timestamp_ms(dongle->sim), dongle->id);
			pthread_mutex_unlock(&dongle->sim->log_mutex);
			break; 
		}
	}
	return (NULL);
}

void init_coders(t_sim *sim)
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

void init_dongles(t_sim *sim)
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

int is_burnout(t_coder *coder)
{
	return (get_timestamp_ms(coder->sim) >= coder->last_compile_start + coder->sim->args->time_to_burnout);
}

int check_burnout(t_sim *sim)
{
	int i = 0;
	while (i < sim->args->number_of_coders)
	{
		if (is_burnout(&sim->coders[i]))
			return (sim->coders[i].id);
		i++;
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_sim *sim = (t_sim *)arg;
	while (1)
	{
		int burnout_id = check_burnout(sim);
		if (burnout_id)
		{
			print_log(&sim->coders[burnout_id - 1], "is burnout");
			sim->running = 0;
			break ; 
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
	sim.running = 1;
	gettimeofday(&sim.start_time, NULL);
	sim.args = &args;
	sim.coders = malloc(args.number_of_coders * sizeof(t_coder));
	sim.dongles = malloc(args.number_of_coders * sizeof(t_dongle));

	init_coders(&sim);
	init_dongles(&sim);

	pthread_mutex_init(&sim.mutex, NULL);
	pthread_mutex_init(&sim.log_mutex, NULL);
	pthread_cond_init(&sim.cond, NULL);

	pthread_create(&sim.monitor_thread, NULL, monitor_routine, &sim);

	if (sim.args->number_of_coders == 1)
	{
		pthread_create(&sim.coders[0].thread, NULL, single_coder_routine, &sim.coders[0]);
		pthread_create(&sim.dongles[0].thread, NULL, single_dongle_routine, &sim.dongles[0]);
		pthread_join(sim.coders[0].thread, NULL);
		pthread_join(sim.dongles[0].thread, NULL);

		pthread_mutex_destroy(&sim.mutex);
		pthread_mutex_destroy(&sim.log_mutex);
		pthread_cond_destroy(&sim.cond);
	
		free(sim.coders);
		free(sim.dongles);
		return (0);
	}


	int i = 0;
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

	pthread_join(sim.monitor_thread, NULL);

	pthread_mutex_destroy(&sim.mutex);
	pthread_mutex_destroy(&sim.log_mutex);
	pthread_cond_destroy(&sim.cond);

	free(sim.coders);
	free(sim.dongles);
	return (0);
}