/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 16:04:18 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/04 18:27:14 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder_routine.h"

void	*coder_routine(void *v_coder)
{
	t_coder	*coder;

	coder = (t_coder*)v_coder;
	pthread_cond_broadcast(coder->monitor->monitor_cond);
	printf("coder %i start his routine and wait\n", coder->id);
	pthread_mutex_lock(coder->monitor->monitor_mutex);
	coder->status = "READY";
	while (strcmp(coder->monitor->status, "READY") != 0)
	{
		pthread_cond_wait(coder->monitor->monitor_cond, coder->monitor->monitor_mutex);

	}
	printf("coder %i finish waiting\n", coder->id);
	pthread_mutex_unlock(coder->monitor->monitor_mutex);
	printf("coder %i finish routine\n", coder->id);
}

// faire en sorte que N sits between coder number N - 1 and coder number N+ 1.

// creer nb_coders dongle
// faire number_of_compiles_required fois: 
	// faire partir les coders pair avec un dongle (selon le sheduler):
		// afficher: timestamp_in_ms X has taken a dongle
		// chopper 2 dongles
		// afficher: timestamp_in_ms is compiling (pendant time_to_compile ms)
		// lacher un dongle 
		// afficher: timestamp_in_ms is debugging (pendant time_to_debug ms)
		// afficher: timestamp_in_ms is refactoring (pendant time_to_refactor ms)
		// lacher les dongles
		// attendre dongle_cooldown ms
		// si burned out : afficher:  timestamp_in_ms X burned out

// shedluer : FIFO (..)
//			  EDF  (Eraliest deadline first: last_compile_start + time_to_burnout)

