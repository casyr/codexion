/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 08:21:32 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/20 21:56:22 by yriffard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

long	ft_atoi(const char *nptr)
{
	size_t			i;
	size_t			value;
	int				sign;
	unsigned char	*uns_nptr;

	uns_nptr = (unsigned char *)nptr;
	sign = 1;
	i = 0;
	value = 0;
	while (((uns_nptr[i] == ' ' || (uns_nptr[i] >= '\t' && uns_nptr[i] <= '\r'))
			&& uns_nptr[i]))
		i++;
	if (uns_nptr[i] == '-' || uns_nptr[i] == '+')
	{
		if (uns_nptr[i] == '-')
			sign = -1;
		i++;
	}
	while (uns_nptr[i] && (uns_nptr[i] >= '0' && uns_nptr[i] <= '9'))
	{
		value = value * 10 + (uns_nptr[i] - '0');
		i++;
	}
	return (value * sign);
}

int	is_int(char *str)
{
	if (ft_atoi(str) >= 2147483648 || ft_atoi(str) <= -2147483648)
		return (1);
	return (0);
}

int	parsing(int argc, char **argv)
{
	if (argc != 9)
		return (1);
	if (is_number_checker(argv[1]) != 0 || is_int(argv[1]) == 1
		|| atoi(argv[1]) == 0)
		return (2);
	if (is_number_checker(argv[2]) != 0 || is_int(argv[2]) == 1)
		return (3);
	if (is_number_checker(argv[3]) != 0 || is_int(argv[3]) == 1)
		return (4);
	if (is_number_checker(argv[4]) != 0 || is_int(argv[4]) == 1)
		return (5);
	if (is_number_checker(argv[5]) != 0 || is_int(argv[5]) == 1)
		return (6);
	if (is_number_checker(argv[6]) != 0 || is_int(argv[6]) == 1)
		return (7);
	if (is_number_checker(argv[7]) != 0 || is_int(argv[7]) == 1)
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
		printf("the number of args isn't correct.");
	if (parsed_data == 2)
		printf("numbers of coder didn't respect the format (valid int > 0).");
	if (parsed_data == 3)
		printf("time to burnout didn't respect the format.");
	if (parsed_data == 4)
		printf("time to compile didn't respect the format.");
	if (parsed_data == 5)
		printf("time to debug didn't respect the format.");
	if (parsed_data == 6)
		printf("time to refactor didn't respect the format.");
	if (parsed_data == 7)
		printf("number of compiles required didn't respect the format.");
	if (parsed_data == 8)
		printf("dongle cooldown didn't respect the format.");
	if (parsed_data == 9)
		printf("sheduler didn't respect the format.");
	return (parsed_data);
}
