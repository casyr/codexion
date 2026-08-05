/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yriffard <yriffard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 16:04:18 by yriffard          #+#    #+#             */
/*   Updated: 2026/08/05 12:05:00 by yriffard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder_routine.h"

void coder_action(t_coder *coder)
{
	int	compile_nb;

	compile_nb = 0;
	while (compile_nb < coder->monitor->compiling_nb)
	{
		
	}
}

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
	if (coder->id % 2 == 0)
		usleep(1000);
	printf("coder %i finish waiting\n", coder->id);
	pthread_mutex_unlock(coder->monitor->monitor_mutex);
	printf("coder %i finish routine\n", coder->id);
	while (strcmp(coder->monitor->status, "BURNOUT") != 0)
		coder_action(coder);
}



// faire en sorte que N sits between coder number N - 1 and coder number N + 1.

// tester avec 3 coders et 

//FAIT creer nb_coders dongle FAIT
// ckeck si burned out : afficher:  timestamp_in_ms X burned out (geré par le monitor)
	// faire number_of_compiles_required fois: 
		// faire partir les coders pair avec un dongle que apres (selon le scheduler):
		// 1 coder choppe 2 dongles  selon le cooldown et selon le scheduler et !!!!!
		// afficher: timestamp_in_ms X has taken a dongle x2
		// afficher: timestamp_in_ms is compiling (pendant time_to_compile ms)
		// lacher les 2 dongles et reacutalise le temps ou tu les lache
		// afficher: timestamp_in_ms is debugging (pendant time_to_debug ms)
		// afficher: timestamp_in_ms is refactoring (pendant time_to_refactor ms)


// schedluer : FIFO (..)
//			   EDF  (Eraliest deadline first: last_compile_start + time_to_burnout)

