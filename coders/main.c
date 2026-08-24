/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haryandr <haryandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 07:50:24 by haryandr          #+#    #+#             */
/*   Updated: 2026/08/24 09:24:16 by haryandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codex.h"

static void	stop_threads(t_sim *sim)
{
	pthread_mutex_lock(&sim->mutex);
	sim->running = 0;
	pthread_mutex_unlock(&sim->mutex);
	pthread_cond_broadcast(&sim->cond);
}

static void	log_error_threads(t_sim *sim)
{
	pthread_mutex_lock(&sim->log_mutex);
	printf("Error: failed to create threads\n");
	pthread_mutex_unlock(&sim->log_mutex);
}

int	main(int ac, char **av)
{
	t_arg	args;
	t_sim	sim;

	if (!parse_args(&args, ac, av))
		return (1);
	if (!init_sim(&sim, &args))
	{
		printf("Error: failed to malloc\n");
		return (1);
	}
	if (!start_threads(&sim))
	{
		log_error_threads(&sim);
		stop_threads(&sim);
		join_threads(&sim);
		destroy_sim(&sim);
		return (1);
	}
	join_threads(&sim);
	destroy_sim(&sim);
	return (0);
}
