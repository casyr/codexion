/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 15:31:08 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/17 14:01:31 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"

void	ft_usleep(long time_in_ms, t_monitoring *monitor)
{
	long	start_time;

	start_time = ft_get_time();
	while ((ft_get_time() - start_time) < time_in_ms)
	{
		pthread_mutex_lock(monitor->monitor_mutex);
		if (strcmp(monitor->status, "BURNOUT") == 0
			|| strcmp(monitor->status, "FINISH") == 0)
		{
			pthread_mutex_unlock(monitor->monitor_mutex);
			break ;
		}
		pthread_mutex_unlock(monitor->monitor_mutex);
		usleep(500);
	}
}

t_dongle	*first_dongle_chooser(t_coder *coder)
{
	t_dongle	*first_dongle;

	first_dongle = coder->right_dongle;
	if (coder->left_dongle->id < coder->right_dongle->id)
		first_dongle = coder->left_dongle;
	return (first_dongle);
}

t_dongle	*second_dongle_chooser(t_coder *coder)
{
	t_dongle	*second_dongle;

	second_dongle = coder->left_dongle;
	if (coder->left_dongle->id < coder->right_dongle->id)
		second_dongle = coder->right_dongle;
	return (second_dongle);
}

void	coder_set_finish(t_coder *coder)
{
	pthread_mutex_lock(coder->monitor->monitor_mutex);
	coder->status = "FINISH";
	coder->monitor->finished_coders_nb += 1;
	pthread_mutex_unlock(coder->monitor->monitor_mutex);
}
