/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine_utils_2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 14:13:33 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/19 16:00:51 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"

void	print_log(char *string, t_coder *coder)
{
	long	time;

	pthread_mutex_lock(&(coder->monitor->monitor_mutex));
	if (strcmp(coder->monitor->status, "BURNOUT") == 0)
	{
		pthread_mutex_unlock(&(coder->monitor->monitor_mutex));
		return ;
	}
	time = ft_get_time() - coder->monitor->start_time;
	pthread_mutex_unlock(&(coder->monitor->monitor_mutex));
	pthread_mutex_lock(&(coder->monitor->print_mutex));
	printf("%li %i %s\n", time, coder->id, string);
	pthread_mutex_unlock(&(coder->monitor->print_mutex));
}

int	dongles_are_available(t_dongle *first_dongle, t_dongle *second_dongle,
	t_coder *coder)
{
	pthread_mutex_lock(&(coder->monitor->monitor_mutex));
	scheduler_choose_and_update(coder);
	if (coder->monitor->total_compile_counter == 0
		&& coder->left_dongle->is_free == true
		&& coder->right_dongle->is_free == true
		&& is_schedule(coder, first_dongle, second_dongle) == 0)
	{
		first_dongle->is_free = false;
		second_dongle->is_free = false;
		pthread_mutex_unlock(&(coder->monitor->monitor_mutex));
		return (0);
	}
	if (coder->left_dongle->is_free == true
		&& coder->right_dongle->is_free == true
		&& (ft_get_time() - (coder->right_dongle->last_release)
			> coder->monitor->dongle_cooldown)
		&& (ft_get_time() -(coder->left_dongle->last_release)
			> coder->monitor->dongle_cooldown)
		&& is_schedule(coder, first_dongle, second_dongle) == 0)
	{
		first_dongle->is_free = false;
		second_dongle->is_free = false;
		pthread_mutex_unlock(&(coder->monitor->monitor_mutex));
		return (0);
	}
	pthread_mutex_unlock(&(coder->monitor->monitor_mutex));
	return (1);
}

void	coder_bump(t_coder *coder)
{
	if (coder->id % 2 == 0)
		usleep(100);
}
