/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcaillie <rcaillie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 21:07:04 by rcaillie          #+#    #+#             */
/*   Updated: 2024/12/17 21:07:04 by rcaillie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_simulation_over(t_program *program)
{
	int	result;

	pthread_mutex_lock(&program->dead_lock);
	result = program->dead_flag;
	pthread_mutex_unlock(&program->dead_lock);
	return (result);
}

void	print_action(t_philo *philo, char *action)
{
	pthread_mutex_lock(&philo->program->write_lock);
	if (!is_simulation_over(philo->program))
		printf("%zu %d %s\n", get_current_time()
			- philo->program->start_time, philo->id, action);
	pthread_mutex_unlock(&philo->program->write_lock);
}

void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->l_fork);
		print_action(philo, "has taken a fork");
		pthread_mutex_lock(philo->r_fork);
	}
	else
	{
		pthread_mutex_lock(philo->r_fork);
		print_action(philo, "has taken a fork");
		pthread_mutex_lock(philo->l_fork);
	}
}

void	update_philo_state(t_philo *philo)
{
	print_action(philo, "is eating");
	pthread_mutex_lock(&philo->program->dead_lock);
	philo->last_meal = get_current_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->program->dead_lock);
	ft_usleep(philo->program->time_to_eat);
	pthread_mutex_unlock(philo->l_fork);
	pthread_mutex_unlock(philo->r_fork);
}
