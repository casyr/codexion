/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 09:04:49 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/06 17:22:01 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"

int	coder_thread_join(int coders_nb, pthread_t *coder_th)
{
	int coder_index;

	coder_index = 0;
	while (coder_index < coders_nb)
	{
		if (pthread_join(coder_th[coder_index], NULL) != 0)
		{
			printf("coder %d thread JOIN fails\n", coder_index);
			return (1);
		}
		coder_index++;
	}
	return(0);
}

t_coder	*coder_list_init(int coders_nb, t_monitoring *monitor, t_dongle *dongle_list)
{
	t_coder			*coder_list;
	int				coder_index;    

	coder_list = malloc(sizeof(t_coder) * coders_nb);
	if (!coder_list)
		return (NULL);
	coder_index = 0;
	while (coder_index < coders_nb)
	{
		coder_list[coder_index].id = coder_index + 1;
		coder_list[coder_index].compile_count = 0;
		coder_list[coder_index].status = "init";
		coder_list[coder_index].monitor = monitor;
		coder_list[coder_index].right_dongle = malloc(sizeof(t_dongle));
		coder_list[coder_index].left_dongle = malloc(sizeof(t_dongle));
		coder_list[coder_index].right_dongle = &(dongle_list[coder_index]);
		if (coder_index == 0)
			coder_list[coder_index].left_dongle = &(dongle_list[-1]);
		else
			coder_list[coder_index].left_dongle = &(dongle_list[coder_index - 1]);
		coder_index++;
	}
	return (coder_list);
}

int	coder_th_creation(t_coder *coder_list, int coders_nb, pthread_t *coder_th, t_monitoring *monitor)
{
	int				coder_index;
	pthread_cond_t	coder_cond;

	coder_index = 0;
	while (coder_index < coders_nb)
	{
		if (pthread_create(&(coder_th[coder_index]), NULL, &coder_routine, &(coder_list[coder_index])) != 0)
		{
			printf("coder %i thread CREATION fails\n", coder_index);
			return (1);
		}

		coder_index++;
	}
	pthread_mutex_lock(monitor->monitor_mutex);
	monitor->status = "READY";
	pthread_mutex_unlock(monitor->monitor_mutex);
	pthread_cond_signal(monitor->monitor_cond);
	return (0);
}
