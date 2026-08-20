#include "codex.h"

void	request_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&coder->sim->mutex);
	heap_push(coder, &dongle->priority);
	pthread_cond_broadcast(&coder->sim->cond);
	pthread_mutex_unlock(&coder->sim->mutex);
}

int pick_up_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&coder->sim->mutex);
	while (coder->sim->running && (dongle->available == 0 || coder != dongle->priority.queue[0].coder))
		pthread_cond_wait(&coder->sim->cond, &coder->sim->mutex);
	if (!coder->sim->running)
	{
		pthread_mutex_unlock(&coder->sim->mutex);
		return (0);
	}
	heap_pop(&dongle->priority);
	dongle->available = 0;
	pthread_mutex_unlock(&coder->sim->mutex);
	log_action(coder, "has taken dongle");
	return (1);
}

int acquire_dongles(t_coder *coder)
{
	if (coder->sim->args->number_of_coders == 1)
	{
		request_dongle(coder, coder->left);
		return (pick_up_dongle(coder, coder->left));
	}
	if (coder->id % 2)
	{
		request_dongle(coder, coder->left);
		if (!pick_up_dongle(coder, coder->left))
			return (0);
		request_dongle(coder, coder->right);
		if (!pick_up_dongle(coder, coder->right))
			return (0);
	}
	else
	{
		request_dongle(coder, coder->right);
		if (!pick_up_dongle(coder,coder->right))
			return (0);
		request_dongle(coder, coder->left);
		if (!pick_up_dongle(coder, coder->left))
			return (0);
	}
	return (1);
}

void compile(t_coder *coder)
{
	log_action(coder, "is compiling");
	pthread_mutex_lock(&coder->sim->mutex);
	coder->last_compile_start = get_timestamp_ms(coder->sim);
	pthread_mutex_unlock(&coder->sim->mutex);
	usleep(coder->sim->args->time_to_compile * 1000);
	pthread_mutex_lock(&coder->sim->mutex);
	coder->compilations_done++;
	pthread_mutex_unlock(&coder->sim->mutex);
}

void	release_dongles(t_coder *coder)
{
	pthread_mutex_lock(&coder->sim->mutex);
	if (coder->sim->args->number_of_coders == 1)
		coder->left->hot = 1;
	else
	{
		coder->left->hot = 1;
		coder->right->hot = 1;			
	}
	if (coder->compilations_done >= coder->sim->args->number_of_compiles_required)
		coder->state = DONE;
	pthread_cond_broadcast(&coder->sim->cond);
	pthread_mutex_unlock(&coder->sim->mutex);
}

void	debug_and_refactor(t_coder *coder)
{
	if (!is_running(coder->sim))
		return ;
	log_action(coder, "is debugging");
	usleep(coder->sim->args->time_to_debug * 1000);
	if (!is_running(coder->sim))
		return ;
	log_action(coder, "is refactoring");
	usleep(coder->sim->args->time_to_refactor * 1000);
}

void *coder_routine(void *arg)
{
	t_coder *coder = (t_coder *)arg;

	while (is_running(coder->sim))
	{
		if (!acquire_dongles(coder))
			break ;
		compile(coder);
		release_dongles(coder);
		if (coder->state == DONE)
		{
			log_action(coder, "is done");
			break ;
		}
		if (!is_running(coder->sim))
			break ;
		debug_and_refactor(coder);
	}
	return (NULL);
}

int	cool_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->sim->mutex);
	if (dongle->sim->args->number_of_coders == 1)
	{
		while (dongle->sim->running && dongle->hot == 0)
			pthread_cond_wait(&dongle->sim->cond, &dongle->sim->mutex);
	}
	else
	{
		while (dongle->sim->running && dongle->hot == 0 && (dongle->users[0]->state != DONE || dongle->users[1]->state != DONE))
			pthread_cond_wait(&dongle->sim->cond, &dongle->sim->mutex);
	}
	pthread_mutex_unlock(&dongle->sim->mutex);
	if (!is_running(dongle->sim))
		return (0);
	usleep(dongle->sim->args->dongle_cooldown * 1000);

	pthread_mutex_lock(&dongle->sim->mutex);
	dongle->hot = 0;
	dongle->available = 1;
	pthread_mutex_unlock(&dongle->sim->mutex);
	return (1);
}

void	*dongle_routine(void *arg)
{
	t_dongle *dongle;
	int	both_done;
	
	dongle = (t_dongle *)arg;
	while (is_running(dongle->sim))
	{
		if (!cool_dongle(dongle))
			break;

		pthread_mutex_lock(&dongle->sim->mutex);
		if (dongle->sim->args->number_of_coders == 1)
			both_done = dongle->users[0]->state == DONE;
		else
			both_done = (dongle->users[0]->state == DONE && dongle->users[1]->state == DONE);
		pthread_cond_broadcast(&dongle->sim->cond);
		pthread_mutex_unlock(&dongle->sim->mutex);

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
	if (coder->state != WORKING)
		return (0);
	return (get_timestamp_ms(coder->sim) >= coder->last_compile_start + coder->sim->args->time_to_burnout);
}

int check_burnout(t_sim *sim)
{
	int i = 0;
	int check;

	while (i < sim->args->number_of_coders)
	{
		pthread_mutex_lock(&sim->mutex);
		check = is_burnout(&sim->coders[i]);
		pthread_mutex_unlock(&sim->mutex);
		if (check)
			return (sim->coders[i].id);
		i++;
	}
	return (0);
}

int	all_done(t_sim *sim)
{
	int i;
	int done;

	i= 0;
	done = 1;
	pthread_mutex_lock(&sim->mutex);
	while (i < sim->args->number_of_coders)
	{
		if (sim->coders[i].state == WORKING)
		{
			done = 0;
			break ;
		}
		i++;
	}
	pthread_mutex_unlock(&sim->mutex);
	return (done);
}

void	*monitor_routine(void *arg)
{
	t_sim *sim;
	int burnout_id;
	
	sim = (t_sim *)arg;
	while (is_running(sim))
	{
		burnout_id = check_burnout(sim);
		if (burnout_id)
		{
			pthread_mutex_lock(&sim->mutex);
			sim->running = 0;
			sim->coders[burnout_id - 1].state = BURNED_OUT;
			log_action(&sim->coders[burnout_id - 1], "is burnout");
			pthread_cond_broadcast(&sim->cond);
			pthread_mutex_unlock(&sim->mutex);
			break ;
		}
		if (all_done(sim))
		{
			pthread_mutex_lock(&sim->mutex);
			sim->running = 0;
			pthread_cond_broadcast(&sim->cond);
			pthread_mutex_unlock(&sim->mutex);
			break ;
		}
		usleep(1000);
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
		pthread_create(&sim.coders[0].thread, NULL, coder_routine, &sim.coders[0]);
		pthread_create(&sim.dongles[0].thread, NULL, dongle_routine, &sim.dongles[0]);
		pthread_join(sim.coders[0].thread, NULL);
		pthread_join(sim.dongles[0].thread, NULL);
		pthread_join(sim.monitor_thread, NULL);

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