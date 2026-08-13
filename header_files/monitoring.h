/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 16:13:43 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/13 18:40:53 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MONITORING_H
# define MONITORING_H

# include "coder.h"
# include <pthread.h>
# include <stdlib.h>
# include "sys/time.h"
# include <string.h>
# include <unistd.h>

typedef struct coder	t_coder;
typedef struct s_dongle	t_dongle;

typedef struct monitor
{
	long			last_compile;
	long			time_to_burnout;
	int				compiling_nb;
	int				coders_nb;
	int				time_to_compile;
	int				time_to_refactor;
	int				time_to_debug;
	int				dongle_cooldown;
	char			*scheduler;
	long			start_time;
	long			last_dongle_release;
	int				finished_coders_nb;
	int				total_compile_counter;

	pthread_t		monitor_th;
	pthread_t		*coder_th;

	t_coder			*coder_list;

	pthread_mutex_t	*print_mutex;
	char			*status;
	pthread_mutex_t	*monitor_mutex;
	pthread_cond_t	*monitor_cond;
	t_dongle		*dongle_list;
}	t_monitoring;

void			*monitoring_routine(void *monitor);
void			print_log(char *string, t_coder *coder);

int				monitoring_th_creation(t_monitoring *monitor);
int				coder_th_creation(t_coder *coder_list, int coders_nb,
					pthread_t *coder_th, t_monitoring *monitor);
int				monitor_thread_join(pthread_t monitoring_th);
t_monitoring	*monitoring_init(char **argv, t_dongle *dongle_list,
					long start_time, t_monitoring *monitor);
long			ft_get_time(void);
int				coder_are_ready(t_coder *coder_list, int coder_nb);

#endif