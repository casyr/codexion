/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine_utils_2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 14:13:33 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/20 15:24:58 by yriffard         ###   ########.fr       */
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

void	enter_dongle_queue(t_dongle *dongle, int coder_id)
{
	if (dongle->queue[0] == coder_id || dongle->queue[1] == coder_id)
		return ;
	if (dongle->queue[0] == 0)
		dongle->queue[0] = coder_id;
	else if (dongle->queue[1] == 0)
		dongle->queue[1] = coder_id;
}

void	leave_dongle_queue(t_dongle *dongle, int coder_id)
{
	if (dongle->queue[0] == coder_id)
	{
		dongle->queue[0] = dongle->queue[1];
		dongle->queue[1] = 0;
	}
	else if (dongle->queue[1] == coder_id)
	{
		dongle->queue[1] = 0;
	}
}

int	are_available_cond(t_dongle *first_dongle, t_dongle *second_dongle,
	t_coder *coder)
{
	if (coder->monitor->total_compile_counter == 0)
	{
		return (coder->left_dongle->is_free == true
			&& coder->right_dongle->is_free == true
			&& are_schedule(coder, first_dongle, second_dongle));
	}
	return (coder->left_dongle->is_free == true
		&& coder->right_dongle->is_free == true
		&& are_schedule(coder, first_dongle, second_dongle)
		&& (ft_get_time() - (coder->right_dongle->last_release)
			> coder->monitor->dongle_cooldown)
		&& (ft_get_time() -(coder->left_dongle->last_release)
			> coder->monitor->dongle_cooldown));
}

int	dongles_are_available(t_dongle *first_dongle, t_dongle *second_dongle,
	t_coder *coder)
{
	pthread_mutex_lock(&(first_dongle->dongle_mutex));
	pthread_mutex_lock(&(second_dongle->dongle_mutex));
	pthread_mutex_lock(&(coder->monitor->monitor_mutex));
	enter_dongle_queue(coder->left_dongle, coder->id);
	enter_dongle_queue(coder->right_dongle, coder->id);
	if (are_available_cond(first_dongle, second_dongle, coder))
	{
		first_dongle->is_free = false;
		second_dongle->is_free = false;
		pthread_mutex_unlock(&(coder->monitor->monitor_mutex));
		return (0);
	}
	if (are_available_cond(first_dongle, second_dongle, coder))
	{
		first_dongle->is_free = false;
		second_dongle->is_free = false;
		pthread_mutex_unlock(&(coder->monitor->monitor_mutex));
		return (0);
	}
	pthread_mutex_unlock(&(first_dongle->dongle_mutex));
	pthread_mutex_unlock(&(second_dongle->dongle_mutex));
	pthread_mutex_unlock(&(coder->monitor->monitor_mutex));
	return (1);
}
