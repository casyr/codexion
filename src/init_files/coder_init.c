/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 09:04:49 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/14 11:26:33 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"

t_coder	*coder_list_init(int coders_nb, t_dongle *dongle_list,
	t_monitoring *monitor)
{
	t_coder			*coder_list;
	int				coder_index;
	t_coder			*coder;

	coder_list = malloc(sizeof(t_coder) * coders_nb);
	if (!coder_list)
		return (NULL);
	coder_index = 0;
	while (coder_index < coders_nb)
	{
		coder = &(coder_list[coder_index]);
		coder->id = coder_index + 1;
		coder->compile_count = 0;
		coder->status = "init";
		coder->right_dongle = &(dongle_list[coder_index]);
		coder->last_compile = ft_get_time();
		coder->monitor = monitor;
		coder->left_dongle = &(dongle_list[coder_index - 1]);
		if (coder_index == 0)
			coder->left_dongle = &(dongle_list[coders_nb - 1]);
		coder_index++;
	}
	return (coder_list);
}

int	coder_th_creation(t_monitoring *monitor)
{
	int				coder_index;

	monitor->coder_th = malloc(monitor->coders_nb * sizeof(pthread_t));
	if (!monitor->coder_th)
	{
		printf("malloc fails");
		return (3);
	}
	monitor->coder_th = monitor->coder_th;
	coder_index = 0;
	while (coder_index < monitor->coders_nb)
	{
		if (pthread_create(&(monitor->coder_th[coder_index]), NULL,
				&coder_routine, &(monitor->coder_list[coder_index])) != 0)
		{
			printf("coder %i thread CREATION fails\n", coder_index);
			return (1);
		}
		coder_index++;
	}
	pthread_mutex_lock(monitor->monitor_mutex);
	monitor->status = "READY";
	pthread_cond_signal(monitor->monitor_cond);
	pthread_mutex_unlock(monitor->monitor_mutex);
	return (0);
}
