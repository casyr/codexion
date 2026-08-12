/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_checker.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 12:07:30 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/12 17:19:13 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	scheduler_checker(char *argv)

{
	if (strcmp(argv, "fifo") == 0 || strcmp(argv, "edf") == 0)
		return (0);
	return (1);
}
