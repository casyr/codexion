/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 11:01:45 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/05 16:55:37 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dongle.h"

t_dongle	*dongle_list_init(int coders_nb)
{
	t_dongle		*dongle_list;
	pthread_mutex_t	dongle_mutex;
	int				i;

	i = 0;
	dongle_list = malloc(sizeof(t_dongle) * coders_nb);
	while (i < coders_nb)
	{
		dongle_list[i].id = i + 1;
		dongle_list[i].is_free = true;
		pthread_mutex_init(&dongle_mutex, NULL);
		dongle_list[i].dongle_mutex = &dongle_mutex;
		i++;
	}
	return (dongle_list);
}
