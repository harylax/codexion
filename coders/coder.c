#include "codex.h"

static void	compile(t_coder *coder)
{
	log_action(coder, "is compiling");
	pthread_mutex_lock(&coder->sim->mutex);
	coder->last_compile_start = get_timestamp_ms(coder->sim);
	pthread_mutex_unlock(&coder->sim->mutex);
	wait_timeout(coder->sim, coder->sim->args->time_to_compile);
	if (is_running(coder->sim) == 0)
		return ;
	pthread_mutex_lock(&coder->sim->mutex);
	coder->compilations_done++;
	pthread_mutex_unlock(&coder->sim->mutex);
}

static void	release_dongles(t_coder *coder)
{
	int	compiles_required;

	pthread_mutex_lock(&coder->left->mutex);
	coder->left->hot = 1;
	pthread_cond_broadcast(&coder->left->cond);
	pthread_mutex_unlock(&coder->left->mutex);
	pthread_mutex_lock(&coder->right->mutex);
	coder->right->hot = 1;
	pthread_cond_broadcast(&coder->right->cond);
	pthread_mutex_unlock(&coder->right->mutex);
	compiles_required = coder->sim->args->number_of_compiles_required;
	pthread_mutex_lock(&coder->sim->mutex);
	if (coder->compilations_done >= compiles_required)
		coder->state = DONE;
	pthread_cond_broadcast(&coder->sim->cond);
	pthread_mutex_unlock(&coder->sim->mutex);
}

static void	debug_and_refactor(t_coder *coder)
{
	if (is_running(coder->sim) == 0)
		return ;
	log_action(coder, "is debugging");
	wait_timeout(coder->sim, coder->sim->args->time_to_debug);
	if (is_running(coder->sim) == 0)
		return ;
	log_action(coder, "is refactoring");
	wait_timeout(coder->sim, coder->sim->args->time_to_refactor);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (is_running(coder->sim))
	{
		if (!acquire_dongles(coder))
			break ;
		if (is_running(coder->sim) == 0)
			break ;
		compile(coder);
		release_dongles(coder);
		debug_and_refactor(coder);
		if (is_running(coder->sim) == 0)
			break ;
		pthread_mutex_lock(&coder->sim->mutex);
		if (coder->state == DONE)
		{
			pthread_mutex_unlock(&coder->sim->mutex);
			break ;
		}
		pthread_mutex_unlock(&coder->sim->mutex);
	}
	return (NULL);
}
