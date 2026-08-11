/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 11:01:45 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/11 11:26:16 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dongle.h"

t_dongle	*dongle_list_init(int coders_nb)
{
	t_dongle		*dongle_list;
	pthread_mutex_t	*dongle_mutex;
	int				i;

	i = 0;
	dongle_mutex = NULL;
	dongle_list = NULL;
	dongle_list = malloc(sizeof(t_dongle) * coders_nb);
	if (!dongle_list)
		return (NULL);
	while (i < coders_nb)
	{
		dongle_list[i].id = i + 1;
		dongle_list[i].is_free = true;
		dongle_list[i].dongle_mutex = malloc(sizeof(pthread_mutex_t));
		dongle_list[i].last_release = ft_get_time();
		if (!dongle_list[i].dongle_mutex)
		{
			while (i >= 0)
			{
				pthread_mutex_destroy(dongle_list[i].dongle_mutex);
				free(dongle_list[i].dongle_mutex);
				i--;
			}
			free(dongle_list);
			return (NULL);
		}
		pthread_mutex_init(dongle_list[i].dongle_mutex, NULL);
		i++;
	}
	return (dongle_list);
}
