#include "codex.h"

static t_request	create_request(t_coder *coder)
{
	long		time_to_burnout;
	long		arrival;
	long		deadline;
	t_request	request;

	time_to_burnout = (long)coder->sim->args->time_to_burnout;
	arrival = get_timestamp_ms(coder->sim);
	deadline = coder->last_compile_start + time_to_burnout;
	request.arrival = arrival;
	request.deadline = deadline;
	request.coder = coder;
	return (request);
}

static void	fifo_push(t_coder *coder, t_heap *heap)
{
	t_request	request;

	request = create_request(coder);
	if (heap->queue[0].arrival > request.arrival)
	{
		heap->queue[1] = heap->queue[0];
		heap->queue[0] = request;
		heap->size++;
	}
	else
	{
		heap->queue[1] = request;
		heap->size++;
	}
}

static void	edf_push(t_coder *coder, t_heap *heap)
{
	t_request	request;

	request = create_request(coder);
	if (coder->compilations_done == 0)
		fifo_push(coder, heap);
	else if (heap->queue[0].deadline > request.deadline)
	{
		heap->queue[1] = heap->queue[0];
		heap->queue[0] = request;
		heap->size++;
	}
	else
	{
		heap->queue[1] = request;
		heap->size++;
	}
}

void	heap_push(t_coder *coder, t_heap *heap)
{
	if (heap->size == 0)
	{
		heap->queue[0] = create_request(coder);
		heap->size++;
		return ;
	}
	if (coder->sim->args->scheduler == FIFO)
		fifo_push(coder, heap);
	else
		edf_push(coder, heap);
}

void	heap_pop(t_heap *heap)
{
	if (heap->size == 0)
		return ;
	heap->queue[0] = heap->queue[1];
	heap->size--;
}
