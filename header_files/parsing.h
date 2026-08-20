/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 16:13:48 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/20 21:56:14 by yriffard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>

int	parsing(int argc, char **argv);
int	parsing_message(int argc, char **argv);
int	is_number_checker(char *argv);
int	scheduler_checker(char *argv);
#endif