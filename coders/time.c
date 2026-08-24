/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haryandr <haryandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 08:45:57 by haryandr          #+#    #+#             */
/*   Updated: 2026/08/24 07:52:50 by haryandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codex.h"

long	get_timestamp_ms(t_sim *sim)
{
	struct timeval	now;
	long			elapsed;

	gettimeofday(&now, NULL);
	elapsed = (now.tv_sec - sim->start_time.tv_sec) * 1000;
	elapsed += (now.tv_usec - sim->start_time.tv_usec) / 1000;
	return (elapsed);
}

void	wait_timeout(t_sim *sim, long timeout)
{
	long	start;
	long	now;

	start = get_timestamp_ms(sim);
	while (is_running(sim))
	{
		now = get_timestamp_ms(sim);
		if (now - start >= timeout)
			break ;
		usleep(100);
	}
}
