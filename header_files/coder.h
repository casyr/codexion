/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 16:09:14 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/17 19:02:48 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODER_H
# define CODER_H

# include "monitoring.h"
# include "dongle.h"
# include <pthread.h>
# include "stdio.h"

typedef struct monitor	t_monitoring;

typedef struct coder
{
	char			*status;
	int				id;
	t_monitoring	*monitor;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	int				compile_count;
	long			last_compile;
}	t_coder;

long		ft_get_time(void);
void		*coder_routine(void *v_coder);
void		print_log(char *string, t_coder *coder);
void		ft_usleep(long time_in_ms, t_monitoring *monitor);
int			is_schedule(t_coder *coder, t_dongle *first_dongle,
				t_dongle *second_dongle);
void		scheduler_choose_and_update(t_coder *coder);

void		edf_queue(t_coder *coder);
void		fifo_queue(t_coder *coder);
int			coder_thread_join(int coders_nb, pthread_t *coder_th);
t_coder		*coder_list_init(int coders_nb, t_dongle *dongle_list,
				t_monitoring *monitor);

t_dongle	*first_dongle_chooser(t_coder *coder);
t_dongle	*second_dongle_chooser(t_coder *coder);
void		coder_set_finish(t_coder *coder);

int			dongles_are_available(t_dongle *first_dongle,
				t_dongle *second_dongle, t_coder *coder);

void		coder_bump(t_coder *coder);

#endif
