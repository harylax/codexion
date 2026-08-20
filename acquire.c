#include "codex.h"

static void	request_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&coder->sim->mutex);
	heap_push(coder, &dongle->priority);
	pthread_cond_broadcast(&coder->sim->cond);
	pthread_mutex_unlock(&coder->sim->mutex);
}

static int pick_up_dongle(t_coder *coder, t_dongle *dongle)
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
	log_action(coder, "has taken a dongle");
	return (1);
}

static int take_pair(t_coder *coder, t_dongle *first, t_dongle *second)
{
	request_dongle(coder, first);
	if (!pick_up_dongle(coder, first))
		return (0);
	request_dongle(coder, second);
	if (!pick_up_dongle(coder, second))
		return (0);
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
		return (take_pair(coder, coder->left, coder->right));
	return (take_pair(coder, coder->right, coder->left));
}
