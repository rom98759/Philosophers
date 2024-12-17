/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_master.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcaillie <rcaillie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 21:05:10 by rcaillie          #+#    #+#             */
/*   Updated: 2024/12/17 21:05:10 by rcaillie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_philo_death(t_program *program, int i)
{
	pthread_mutex_lock(&program->dead_lock);
	if ((get_current_time() - program->philos[i].last_meal)
		> (size_t)program->time_to_die)
	{
		program->dead_flag = 1;
		pthread_mutex_lock(&program->write_lock);
		printf("%zu %d died\n", get_current_time() - program->start_time,
			program->philos[i].id);
		pthread_mutex_unlock(&program->write_lock);
		pthread_mutex_unlock(&program->dead_lock);
		return (1);
	}
	pthread_mutex_unlock(&program->dead_lock);
	return (0);
}

static int	check_all_philos_full(t_program *program)
{
	int	i;
	int	all_ate;

	i = 0;
	all_ate = 1;
	while (i < program->nb_philos)
	{
		pthread_mutex_lock(&program->dead_lock);
		if (program->philos[i].meals_eaten < program->max_meals
			|| program->max_meals == -1)
			all_ate = 0;
		pthread_mutex_unlock(&program->dead_lock);
		i++;
	}
	return (all_ate);
}

void	*game_master(void *arg)
{
	t_program	*program;
	int			i;

	program = (t_program *)arg;
	while (1)
	{
		i = -1;
		while (++i < program->nb_philos)
			if (check_philo_death(program, i))
				return (NULL);
		if (check_all_philos_full(program))
		{
			pthread_mutex_lock(&program->write_lock);
			pthread_mutex_lock(&program->dead_lock);
			program->dead_flag = 1;
			printf("All philosophers have eaten the maximum number of meals\n");
			pthread_mutex_unlock(&program->dead_lock);
			pthread_mutex_unlock(&program->write_lock);
			break ;
		}
		usleep(50);
	}
	return (NULL);
}
