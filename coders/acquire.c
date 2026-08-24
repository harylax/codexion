#include "codex.h"

static int	waiting_conditions(t_coder *coder, t_dongle *dongle)
{
	if (dongle->available == 1)
	{
		if (is_first_in_queue(coder, dongle))
			return (0);
	}
	return (1);
}

static int	pick_up_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	while (waiting_conditions(coder, dongle))
	{
		if (is_running(coder->sim) == 0)
			break ;
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
	}
	if (is_running(coder->sim) == 0)
	{
		pthread_mutex_unlock(&dongle->mutex);
		return (0);
	}
	heap_pop(&dongle->priority);
	dongle->available = 0;
	pthread_mutex_unlock(&dongle->mutex);
	if (is_running(coder->sim) == 0)
		return (0);
	log_action(coder, "has taken a dongle");
	return (1);
}

static int	take_pair(t_coder *coder, t_dongle *first, t_dongle *second)
{
	request_dongle(coder, first);
	request_dongle(coder, second);
	if (pick_up_dongle(coder, first) == 0)
		return (0);
	if (pick_up_dongle(coder, second) == 0)
		return (0);
	return (1);
}

int	one_coder_burnout(t_coder *coder)
{
	request_dongle(coder, coder->left);
	request_dongle(coder, coder->left);
	if (pick_up_dongle(coder, coder->left) == 0)
		return (0);
	if (pick_up_dongle(coder, coder->left) == 0)
		return (0);
	return (1);
}

int	acquire_dongles(t_coder *coder)
{
	if (coder->sim->args->number_of_coders == 1)
		return (one_coder_burnout(coder));
	if (coder->id % 2)
		return (take_pair(coder, coder->left, coder->right));
	return (take_pair(coder, coder->right, coder->left));
}
