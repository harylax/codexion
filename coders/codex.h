#ifndef CODEX_H
# define CODEX_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>

typedef enum e_scheduler
{
	FIFO,
	EDF,
}	t_scheduler;

typedef enum e_state
{
	WORKING,
	DONE,
	BURNED_OUT
}	t_state;

typedef enum e_end
{
	SUCCESS,
	FAIL
}	t_end;

typedef struct s_arg
{
	int			number_of_coders;
	int			time_to_burnout;
	int			time_to_compile;
	int			time_to_debug;
	int			time_to_refactor;
	int			number_of_compiles_required;
	int			dongle_cooldown;
	t_scheduler	scheduler;
}	t_arg;

typedef struct s_sim	t_sim;
typedef struct s_coder	t_coder;

typedef struct s_request
{
	t_coder	*coder;
	long	arrival;
	long	deadline;
}	t_request;

typedef struct s_heap
{
	t_request	queue[2];
	int			size;
}	t_heap;

typedef struct s_dongle
{
	int				id;
	pthread_t		thread;
	int				available;
	int				hot;
	t_coder			*users[2];
	t_heap			priority;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	t_sim			*sim;
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

typedef struct s_count
{
	int	monitor_thread;
	int	coder_threads;
	int	dongle_threads;
}	t_count;

typedef struct s_sim
{
	int				running;
	t_dongle		*dongles;
	t_coder			*coders;
	t_arg			*args;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	struct timeval	start_time;
	pthread_mutex_t	log_mutex;
	pthread_t		monitor_thread;
	t_end			end;
	t_count			count;
}	t_sim;

int		parse_args(t_arg *args, int ac, char **av);
long	get_timestamp_ms(t_sim *sim);
void	heap_push(t_coder *coder, t_heap *heap);
void	heap_pop(t_heap *heap);
int		is_running(t_sim *sim);
void	log_action(t_coder *coder, char *log);
int		acquire_dongles(t_coder *coder);
void	*coder_routine(void *arg);
void	*dongle_routine(void *arg);
int		init_sim(t_sim *sim, t_arg *arg);
void	destroy_sim(t_sim *sim);
void	*monitor_routine(void *arg);
int		start_threads(t_sim *sim);
void	join_threads(t_sim *sim);
void	wait_timeout(t_sim *sim, long ms);
int		is_first_in_queue(t_coder *coder, t_dongle *dongle);
void	request_dongle(t_coder *coder, t_dongle *dongle);

#endif