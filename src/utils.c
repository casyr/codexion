/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 12:08:48 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/19 13:59:43 by yriffard         ###   ########.fr       */
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

int	coder_are_ready(t_coder *coder_list, int coder_nb)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (i < coder_nb)
	{
		if (strcmp(coder_list[i].status, "READY") == 0)
			count += 1;
		i++;
	}
	if (count == coder_nb)
		return (1);
	return (0);
}

void	free_all(t_monitoring *monitor)
{
	int	i;

	if (monitor)
	{
		i = 0;
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
		if (&(monitor->monitor_cond))
			pthread_cond_destroy(&(monitor->monitor_cond));
		if (&(monitor->monitor_mutex))
			pthread_mutex_destroy(&(monitor->monitor_mutex));
		if (&(monitor->print_mutex))
			pthread_mutex_destroy(&(monitor->print_mutex));
		while (i < monitor->coders_nb)
		{
			if (!monitor->dongle_list)
				break ;
			if (&(monitor->dongle_list[i].dongle_mutex))
				pthread_mutex_destroy(&(monitor->dongle_list[i].dongle_mutex));
			i++;
		}
	}
}

void	destroy_and_free(t_monitoring *monitor)
{
	free_all(monitor);
	destroy_all(monitor);
}
