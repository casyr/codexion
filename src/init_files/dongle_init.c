/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 11:01:45 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/14 11:53:34 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dongle.h"

void	dongle_list_destroy(int i, t_dongle *dongle_list)
{
	while (i > 0)
	{
		i--;
		pthread_mutex_destroy(dongle_list[i].dongle_mutex);
		free(dongle_list[i].dongle_mutex);
	}
	free(dongle_list);
}

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
		if (!dongle_list[i].dongle_mutex)
		{
			dongle_list_destroy(i, dongle_list);
			return (NULL);
		}
		pthread_mutex_init(dongle_list[i].dongle_mutex, NULL);
		dongle_list[i].last_release = ft_get_time();
		i++;
	}
	return (dongle_list);
}
