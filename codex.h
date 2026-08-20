#ifndef CODEX_H
#define CODEX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

typedef enum s_scheduler
{
	FIFO,
	EDF,
}	t_scheduler;

typedef enum s_state
{
	WORKING,
	DONE,
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
typedef struct s_coder t_coder;

typedef struct s_request
{
	t_coder 			*coder;
	long 				arrival;
	long 				deadline;
}	t_request;

typedef struct s_heap
{
	t_request 	queue[2];
	int			size;
}	t_heap;


typedef struct s_dongle
{
	int			id;
	pthread_t 	thread;
	int 		available;
	int 		hot;
	t_coder 	*users[2];
	t_heap		priority;
	t_sim 		*sim;
}	t_dongle;

typedef struct s_coder
{
	int			id;
	pthread_t	thread;
	int			compilations_done;
	long		last_compile_start;
	t_state		state;
	t_dongle	*left;
	t_dongle	*right;
	t_sim		*sim;
}	t_coder;

typedef struct s_sim
{
	int				running;
	t_dongle 		*dongles;
	t_coder			*coders;
	t_arg			*args;
	pthread_mutex_t mutex;
	pthread_cond_t	cond;
	struct timeval 	start_time;
	pthread_mutex_t log_mutex;
	t_heap 			*queue;
	pthread_t		monitor_thread;
}	t_sim;

int missing_args(int ac);
int check_args(int ac, char **av);
void parse_args(t_arg *args, char **av);
long get_timestamp_ms(t_sim *sim);
void heap_push(t_coder *coder, t_heap *heap);
void heap_pop(t_heap *heap);
int is_running(t_sim *sim);
void	log_action(t_coder *coder, char *log);


#endif