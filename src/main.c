/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 08:21:18 by yriffard          #+#    #+#             */
/*   Updated: 2026/06/24 13:49:57 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int		parsing(int argc, char **argv)
{
	int	parsed_data;

	parsed_data = parser(argc, argv);
	if (parsed_data == 1)
		printf("the number of args isn't correct.");
	if (parsed_data == 2)
		printf("numbers of coder didn't respect the format.");
	if (parsed_data == 3)
		printf("time to burnout didn't respect the format.");
	if (parsed_data == 4)
		printf("time to compile didn't respect the format.");
	if (parsed_data == 5)
		printf("time to debug didn't respect the format.");
	if (parsed_data == 6)
		printf("time to refactor didn't respect the format.");
	if (parsed_data == 7)
		printf("number of compiles required didn't respect the format.");
	if (parsed_data == 8)
		printf("dongle cooldown didn't respect the format.");
	if (parsed_data == 9)
		printf("sheduler didn't respect the format."); 
	return (parsed_data);
}

void	*routine()
{
	printf("la\n");
}

void	*monitoring_routine(void* monitor)
{
	int 			compiling_counter;
	int 			time;
	struct timeval	current_time;
	t_monitoring 	monitoring;

	compiling_counter = 0;
	while(1)
	{
		time = gettimeofday(&current_time, NULL);
		if (time + monitoring.last_compile > time + monitoring.time_to_burnout)
			printf("BURNOUT!");
		printf("time: %d", time);
	}
}

// void	monitoring_init(t_monitoring *monitor, )
// {
// }

// void coder_init(t_coder *coder)
// {
	
// }

int		main(int argc, char **argv)
{
	int 			parsed_data;
	int 			coders_nb;
	int 			coder_index;
	int				compiling_nb;
	pthread_t 		*coder_th;
	pthread_t		monitoring_th;
	t_monitoring	monitor;
	t_coder			coder;
	struct timeval	current_time;

	coder_index = 0;
	parsed_data = parsing(argc, argv);
	if (parsed_data != 0)
		return parsed_data;
	coders_nb = atoi(argv[1]);
	compiling_nb = atoi(argv[6]);
	if (coders_nb < 2)
	{
		printf("number of coders must be > 1");
		return (1);
	}
	if (compiling_nb == 0)
	{
		printf("compiling number must be > 0");
		return (1);
	}
	coder_th = malloc(coders_nb * sizeof(pthread_t));
	if (!coder_th)
	{
		printf("malloc fails");
		return (1);
	}
	monitor.compiling_nb = compiling_nb;
	monitor.last_compile = gettimeofday(&current_time, NULL);
	while (coder_index < coders_nb)
	{
		coder.id = coder_index;
		coder.state = "NOT FINISH";
		if(pthread_create(&(coder_th[coder_index]), NULL, &routine, &coder) != 0)
		{
			printf("coder %i thread CREATION fails\n", coder_index);
			return (1);
		}
		coder_index++;
	}
	if(pthread_create(&monitoring_th, NULL, &monitoring_routine, &monitor) != 0)
	{
		printf("monitoring thread CREATION fails");
		return (1);
	}
	while (coder_index < coders_nb)
	{
		if(pthread_join(coder_th[coder_index], NULL) != 0)
		{
			printf("coder %d thread JOIN fails\n", coder_index);
			return (1);
		}
		coder_index++;
	}
	if(pthread_join(monitoring_th, NULL) != 0)
	{
		printf("monitoring thread JOIN fails\n");
		return (1);
	}
	return (0);
}
