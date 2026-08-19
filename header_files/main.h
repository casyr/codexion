/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 16:13:08 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/18 17:56:32 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

# include "parsing.h"
# include "monitoring.h"
# include "coder.h"
# include "dongle.h"

void	free_all(t_monitoring *monitor);
void	destroy_all(t_monitoring *monitor);
void	destroy_and_free(t_monitoring *monitor);
#endif