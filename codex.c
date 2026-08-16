#include <pthread.h>
#include <stdio.h>

typedef struct s_global t_global;

typedef struct s_coder
{
	int 		id;
	pthread_t 	thread;
	t_global	*global;
}	t_coder;

typedef struct s_global
{
	t_coder 		coders[3];
	int				count;
	pthread_mutex_t mutex;
}	t_global;

void *routine(void *arg)
{
	t_coder *coder = (t_coder *)arg;
	int i = 0;
	while (i < 10)
	{
		pthread_mutex_lock(&coder->global->mutex);
		printf("Thread %d: count %d\n", coder->id, coder->global->count);
		coder->global->count++;
		pthread_mutex_unlock(&coder->global->mutex);
		i++;
	}
	pthread_mutex_lock(&coder->global->mutex);
	printf("Total count: thread %d = %d\n", coder->id, coder->global->count);
	pthread_mutex_unlock(&coder->global->mutex);
	return (NULL);
}

int main()
{
	t_global global;
	global.count = 0;
	pthread_mutex_init(&global.mutex, NULL);
	int i = 0;
	while (i < 3)
	{
		global.coders[i].global = &global;
		global.coders[i].id = i;
		i++;
	}
	
	i = 0;
	while (i < 3)
	{
		pthread_create(&global.coders[i].thread, NULL, routine, &global.coders[i]);
		i++;
	}
	
	i = 0;
	while (i < 3)
	{
		pthread_join(global.coders[i].thread, NULL);
		i++;
	}
	pthread_mutex_destroy(&global.mutex);
	return (0);
}
