/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 08:21:32 by yriffard          #+#    #+#             */
/*   Updated: 2026/07/31 06:36:48 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	parsing(int argc, char **argv)
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

int	parsing_message(int argc, char **argv)
{
	int	parsed_data;

	parsed_data = parsing(argc, argv);
	if (parsed_data == 1)
	{
		printf("the number of args isn't correct.");
		return (1);
	}
	if (parsed_data == 2)
	{
		printf("numbers of coder didn't respect the format.");
		return (2);
	}
	if (parsed_data == 3)
	{
		printf("time to burnout didn't respect the format.");
		return (3);
	}
	if (parsed_data == 4)
	{
		printf("time to compile didn't respect the format.");
		return (4);
	}
	if (parsed_data == 5)
	{
		printf("time to debug didn't respect the format.");
		return (5);
	}
	if (parsed_data == 6)
	{
		printf("time to refactor didn't respect the format.");
		return (6);
	}
	if (parsed_data == 7)
	{
		printf("number of compiles required didn't respect the format.");
		return (7);
	}
	if (parsed_data == 8)
	{
		printf("dongle cooldown didn't respect the format.");
		return (8);
	}
	if (parsed_data == 9)
	{
		printf("sheduler didn't respect the format.");
		return (9); 
	}
	return (0);
}
