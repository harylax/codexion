/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   acquire.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haryandr <haryandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 07:48:02 by haryandr          #+#    #+#             */
/*   Updated: 2026/08/24 09:25:44 by haryandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codex.h"

static void	request_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	heap_push(coder, &dongle->priority);
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}

// static int	waiting_conditions(t_coder *coder, t_dongle *dongle)
// {
// 	if (coder->sim->running == 0)
// 		return (0);
// 	if (dongle->available == 1 && is_first_in_queue(coder, dongle))
// 		return (0);
// 	return (1);
// }

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
	log_action(coder, "has taken a dongle");
	return (1);
}

static int	take_pair(t_coder *coder, t_dongle *first, t_dongle *second)
{
	request_dongle(coder, first);
	request_dongle(coder, second);
	if (!pick_up_dongle(coder, first))
		return (0);
	if (!pick_up_dongle(coder, second))
		return (0);
	return (1);
}

int	acquire_dongles(t_coder *coder)
{
	if (coder->id % 2)
		return (take_pair(coder, coder->left, coder->right));
	return (take_pair(coder, coder->right, coder->left));
}
