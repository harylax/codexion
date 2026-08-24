/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haryandr <haryandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 07:49:17 by haryandr          #+#    #+#             */
/*   Updated: 2026/08/24 09:26:01 by haryandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codex.h"

// static int	waiting_conditions(t_dongle *dongle)
// {
// 	if (dongle->sim->running == 0)
// 		return (0);
// 	if (dongle->hot == 1)
// 		return (0);
// 	if (dongle->users[0]->state == DONE && dongle->users[1]->state == DONE)
// 		return (0);
// 	return (1);
// }

static int	waiting_conditions(t_dongle *dongle)
{
	if (dongle->hot == 1)
		return (0);
	pthread_mutex_lock(&dongle->sim->mutex);
	if (dongle->users[0]->state == DONE && dongle->users[1]->state == DONE)
	{
		pthread_mutex_unlock(&dongle->sim->mutex);
		return (0);
	}
	pthread_mutex_unlock(&dongle->sim->mutex);
	return (1);
}

static void	wait_for_hot_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	while (waiting_conditions(dongle))
	{
		if (is_running(dongle->sim))
			break ;
		pthread_cond_wait(&dongle->sim->cond, &dongle->sim->mutex);
	}
	pthread_mutex_unlock(&dongle->mutex);
}

static void	enable_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->hot = 0;
	dongle->available = 1;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}

static int	cool_dongle(t_dongle *dongle)
{
	wait_for_hot_dongle(dongle);
	if (is_running(dongle->sim) == 0)
		return (0);
	wait_timeout(dongle->sim, dongle->sim->args->dongle_cooldown);
	enable_dongle(dongle);
	return (1);
}

void	*dongle_routine(void *arg)
{
	t_dongle	*dongle;

	dongle = (t_dongle *)arg;
	while (is_running(dongle->sim))
	{
		if (!cool_dongle(dongle))
			break ;
		pthread_mutex_lock(&dongle->sim->mutex);
		if (dongle->users[0]->state == DONE && dongle->users[1]->state == DONE)
		{
			pthread_mutex_unlock(&dongle->sim->mutex);
			break ;
		}
		pthread_mutex_unlock(&dongle->sim->mutex);
	}
	return (NULL);
}
