/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 16:13:43 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/21 14:13:50 by yriffard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MONITORING_H
# define MONITORING_H

# include <pthread.h>
# include <stdlib.h>
# include "sys/time.h"
# include <string.h>
# include <unistd.h>
# include <stdio.h>
# include "status.h"
# include "coder.h"

typedef struct coder	t_coder;
typedef struct s_dongle	t_dongle;

typedef struct monitor
{
	long			start_time;

	int				compiling_nb;
	int				coders_nb;
	int				time_to_compile;
	int				time_to_refactor;
	int				time_to_debug;
	int				dongle_cooldown;
	int				time_to_burnout;

	char			*scheduler;
	t_status		status;

	int				finished_coders_nb;
	int				total_compile_counter;

	pthread_t		monitor_th;
	pthread_t		*coder_th;

	t_coder			*coder_list;

	pthread_mutex_t	print_mutex;

	pthread_mutex_t	monitor_mutex;
	pthread_cond_t	monitor_cond;
	t_dongle		*dongle_list;
}	t_monitoring;

void			*monitoring_routine(void *monitor);
void			print_log(char *string, t_coder *coder);

int				coder_th_creation(t_monitoring *monitor);
void			*monitoring_init(char **argv,
					long start_time, t_monitoring *monitor);
long			ft_get_time(void);

#endif