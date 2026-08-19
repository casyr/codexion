/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 11:01:45 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/19 14:05:06 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dongle.h"

void	dongle_list_destroy(int i, t_dongle *dongle_list)
{
	while (i > 0)
	{
		i--;
		pthread_mutex_destroy(&(dongle_list[i].dongle_mutex));
	}
	free(dongle_list);
}

void	dongle_list_init(int coders_nb, t_dongle *dongle_list)
{
	int				i;

	i = 0;
	while (i < coders_nb)
	{
		dongle_list[i].id = i + 1;
		dongle_list[i].is_free = true;
		pthread_mutex_init(&(dongle_list[i].dongle_mutex), NULL);
		dongle_list[i].last_release = ft_get_time();
		dongle_list[i].queue[0] = 0;
		dongle_list[i].queue[1] = 0;
		i++;
	}
}
