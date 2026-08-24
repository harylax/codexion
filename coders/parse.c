/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haryandr <haryandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 07:51:43 by haryandr          #+#    #+#             */
/*   Updated: 2026/08/24 07:51:45 by haryandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codex.h"

static int	check_non_digit(char *s)
{
	int	i;

	i = 0;
	while (s[i] == ' ' || (s[i] >= 9 && s[i] <= 13))
		i++;
	if (s[i] == '+' || s[i] == '-')
		i++;
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

static int	is_overflow(char *s)
{
	int			i;
	long long	res;
	int			sign;

	i = 0;
	res = 0;
	sign = 1;
	while (s[i] == ' ' || (s[i] >= 9 && s[i] <= 13))
		i++;
	if (s[i] == '+' || s[i] == '-')
	{
		if (s[i] == '-')
			sign = -1;
		i++;
	}
	while (s[i] >= '0' && s[i] <= '9')
	{
		res = res * 10 + (s[i] - '0');
		if (res * sign > 2147483647)
			return (1);
		if (res * sign < -2147483648)
			return (-1);
		i++;
	}
	return (0);
}

static int	missing_args(int ac)
{
	if (ac != 9)
	{
		if (ac < 9)
			printf("Error: missing argument(s)\n");
		else if (ac > 9)
			printf("Error: too many argument(s)\n");
		printf("Usage: ./codexion <number_of_coders> <time_to_burnout> ");
		printf("<time_to_compile> <time_to_debug> <time_to_refactor> ");
		printf("<number_of_compiles_required> <dongle_cooldown> <scheduler>\n");
		return (1);
	}
	return (0);
}

static int	check_args(int ac, char **av)
{
	int	err;
	int	i;

	err = 0;
	i = 1;
	while (i < ac - 1)
	{
		if (check_non_digit(av[i]) || is_overflow(av[i]) || atoi(av[i]) < 0)
			err = 1;
		if (check_non_digit(av[i]))
			printf("Error: '%s' is not a valid integer\n", av[i]);
		else if (is_overflow(av[i]) == 1)
			printf("Error: '%s' is too big\n", av[i]);
		else if (atoi(av[i]) < 0 || is_overflow(av[i]) == -1)
			printf("Error: '%s' is a negative integer\n", av[i]);
		i++;
	}
	if (strcmp(av[ac - 1], "fifo") && strcmp(av[ac - 1], "edf"))
	{
		printf("Error: '%s' is not a valid scheduler ", av[ac - 1]);
		printf("(either \"fifo\" or \"edf\")\n");
		err = 1;
	}
	return (err);
}

int	parse_args(t_arg *args, int ac, char **av)
{
	if (missing_args(ac) || check_args(ac, av))
		return (0);
	if (atoi(av[1]) == 0 || atoi(av[2]) == 0 || atoi(av[6]) == 0)
	{
		if (atoi(av[1]) == 0)
			printf("Error: the simulation needs at least one coder\n");
		if (atoi(av[2]) == 0)
			printf("Error: the simulation needs at least time to burnout\n");
		if (atoi(av[6]) == 0)
			printf("Error: the simulation needs at least one compile\n");
		return (0);
	}
	args->number_of_coders = atoi(av[1]);
	args->time_to_burnout = atoi(av[2]);
	args->time_to_compile = atoi(av[3]);
	args->time_to_debug = atoi(av[4]);
	args->time_to_refactor = atoi(av[5]);
	args->number_of_compiles_required = atoi(av[6]);
	args->dongle_cooldown = atoi(av[7]);
	if (strcmp(av[8], "fifo") == 0)
		args->scheduler = FIFO;
	else
		args->scheduler = EDF;
	return (1);
}
