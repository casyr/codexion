/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 12:08:48 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/20 21:56:44 by yriffard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"

long	ft_get_time(void)
{
	struct timeval	current_time;
	long			result;

	if (gettimeofday(&current_time, NULL) != 0)
		return (1);
	result = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	return (result);
}

void	free_all(t_monitoring *monitor)
{
	if (monitor)
	{
		if (monitor->dongle_list)
		{
			free(monitor->dongle_list);
		}
		if (monitor->coder_list)
			free(monitor->coder_list);
		if (monitor->coder_th)
			free(monitor->coder_th);
	}
}

void	destroy_all(t_monitoring *monitor)
{
	int	i;

	i = 0;
	if (monitor)
	{
		pthread_cond_destroy(&(monitor->monitor_cond));
		pthread_mutex_destroy(&(monitor->monitor_mutex));
		pthread_mutex_destroy(&(monitor->print_mutex));
		if (!monitor->dongle_list)
			return ;
		while (i < monitor->coders_nb)
		{
			pthread_mutex_destroy(&(monitor->dongle_list[i].dongle_mutex));
			i++;
		}
	}
}
