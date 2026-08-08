#include "codex.h"

int main(int ac, char **av)
{
	t_arg args;
	if (ac != 9)
	{
		printf("Missing argument(s)\n");
		printf("Usage: ./main <number_of_coders> <time_to_burnout> <time_to_compile> <time_to_debug> <time_to_refactor> <number_of_compiles_required> <dongle_cooldown> <scheduler>\n");
		return (1);
	}
	int flag = 0;
	args.number_of_coders = atoi(av[1]);
	if (args.number_of_coders <= 0)
	{
		printf("number_of_coders should be a positive integer\n");
		flag = 1;
	}
	args.time_to_burnout = atoi(av[2]);
	if (args.time_to_burnout <= 0)
	{
		printf("time_to_burnout should be a positive integer\n");
		flag = 1;
	}
	args.time_to_compile = atoi(av[3]);
	if (args.time_to_compile <= 0)
	{
		printf("time_to_compile should be a positive integer\n");
		flag = 1;
	}
	args.time_to_debug = atoi(av[4]);
	if (args.time_to_debug <= 0)
	{
		printf("time_to_debug should be a positive integer\n");
		flag = 1;
	}
	args.time_to_refactor = atoi(av[5]);
	if (args.time_to_refactor <= 0)
	{
		printf("time_to_refactor should be a positive integer\n");
		flag = 1;
	}
	args.number_of_compiles_required = atoi(av[6]);
	if (args.number_of_compiles_required <= 0)
	{
		printf("number_of_compiles_required should be a positive integer\n");
		flag = 1;
	}
	args.dongle_cooldown = atoi(av[7]);
	if (args.dongle_cooldown <= 0)
	{
		printf("dongle_cooldown should be a positive integer\n");
		flag = 1;
	}
	if (av[8])
	{
		if (strcmp(av[8], "fifo") == 0)
		{
			args.scheduler = FIFO;
		}
		else if (strcmp(av[8], "edf") == 0)
		{
			args.scheduler = EDF;
		}
		else
		{
			printf("scheduler should be either \"fifo\" or \"edf\"\n");
			flag = 1;
		}
	}
	if (flag)
	{
		return (1);
	}
	int i = 0;
	while (i < args.number_of_coders)
	{
		pthread_create()
	}
	printf("number_of_coders=%d\n", args.number_of_coders);
	printf("time_to_burnout=%d\n", args.time_to_burnout);
	printf("time_to_compile=%d\n", args.time_to_compile);
	printf("time_to_debug=%d\n", args.time_to_debug);
	printf("time_to_refactor=%d\n", args.time_to_refactor);
	printf("number_of_compiles_required=%d\n", args.number_of_compiles_required);
	printf("dongle_cooldown=%d\n", args.dongle_cooldown);
	printf("scheduler=%d\n", args.scheduler);
	return (0);
}
