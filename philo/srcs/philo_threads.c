/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_threads.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcaillie <rcaillie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 21:06:17 by rcaillie          #+#    #+#             */
/*   Updated: 2025/02/10 10:48:28 by rcaillie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * Handle the case where there is only one philosopher.
 */
static void	handle_single_philo(t_philo *philo)
{
	print_action(philo, "has taken a fork");
	ft_usleep(philo->program->time_to_die);
}

/**
 * Routine for each philosopher thread.
 */
void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->program->nb_philos == 1)
	{
		handle_single_philo(philo);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		usleep(1);
	while (!is_simulation_over(philo->program))
	{
		take_forks(philo);
		update_philo_state(philo);
		if (is_simulation_over(philo->program))
			break ;
		print_action(philo, "is sleeping");
		ft_usleep(philo->program->time_to_sleep);
		print_action(philo, "is thinking");
	}
	return (NULL);
}
