#ifndef CODEX_H
#define CODEX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

typedef enum s_scheduler
{
	FIFO,
	EDF,
}	t_scheduler;

typedef enum s_state
{
	COMPILING,
	DEBUGGING,
	REFACTORING,
	BURNED_OUT
}	t_state;

typedef struct s_arg
{
	int			number_of_coders;
	int			time_to_burnout;
	int 		time_to_compile;
	int			time_to_debug;
	int			time_to_refactor;
	int			number_of_compiles_required;
	int			dongle_cooldown;
	t_scheduler scheduler;
}	t_arg;

typedef struct s_sim t_sim;

typedef struct s_dongle
{
	int	id;
	pthread_mutex_t mut;
	int available;
	int cooling_down;
}	t_dongle;

typedef struct s_coder
{
	int			id;
	pthread_t	thread;
	int			compilations_done;
	t_state		state;
	t_dongle	*left;
	t_dongle	*right;
	t_sim		*sim;
}	t_coder;

typedef struct s_sim
{
	int	running;
	t_dongle *dongles;
	t_coder	*coders;
	t_arg	*args;
}	t_sim;

int missing_args(int ac);
int check_args(int ac, char **av);
void parse_args(t_arg *args, char **av);

#endif