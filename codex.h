#ifndef CODEX_H
#define CODEX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef enum s_scheduler
{
	FIFO,
	EDF,
}	t_scheduler;

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

typedef struct s_dongle
{
	int	owner;
	int user;
	int cooldown;
}	t_dongle;

typedef struct s_coder
{
	int			id;
	int			compilations_done;
	int			is_compiling;
	int			is_debugging;
	int			is_refactoring;
	int			burned_out;
	t_dongle	dongle;
}	t_coder;

#endif