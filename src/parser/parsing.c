/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 08:21:32 by yriffard          #+#    #+#             */
/*   Updated: 2026/06/23 12:06:24 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	parser(int argc, char **argv)
{
	int	i;

	i = 0;
	if (argc != 9)
		return (1);
	if (is_number_checker(argv[1]) != 0)
		return (2);
	if (is_number_checker(argv[2]) != 0)
		return (3);
	if (is_number_checker(argv[3]) != 0)
		return (4);
	if (is_number_checker(argv[4]) != 0)
		return (5);
	if (is_number_checker(argv[5]) != 0)
		return (6);
	if (is_number_checker(argv[6]) != 0)
		return (7);
	if (is_number_checker(argv[7]) != 0)
		return (8);
	if (scheduler_checker(argv[8]) != 0)
		return (9);
	return (0);
}
