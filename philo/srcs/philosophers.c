/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcaillie <rcaillie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 13:38:05 by rcaillie          #+#    #+#             */
/*   Updated: 2024/12/17 14:08:00 by rcaillie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_philo(t_program *program)
{
	int	i;

	i = -1;
	while (++i < program->nb_philos)
		pthread_mutex_init(&program->forks[i], NULL);
	i = -1;
	while (++i < program->nb_philos)
	{
		program->philos[i].id = i + 1;
		program->philos[i].meals_eaten = 0;
		program->philos[i].last_meal = program->start_time;
		program->philos[i].r_fork = &program->forks[i];
		program->philos[i].l_fork = &program->forks[(i + 1) % program->nb_philos];
		program->philos[i].program = program;
	}
}

int	init_program(t_program *program, int ac, char **av)
{
	program->nb_philos = atoi(av[1]);
	program->time_to_die = atoi(av[2]);
	program->time_to_eat = atoi(av[3]);
	program->time_to_sleep = atoi(av[4]);
	if (ac == 6)
	{
		program->max_meals = atoi(av[5]);
		printf("max_meals = %d\n", program->max_meals);
	}
	else
		program->max_meals = -1;
	program->dead_flag = 0;
	program->start_time = get_current_time();
	program->forks = malloc(sizeof(pthread_mutex_t) * program->nb_philos);
	program->philos = malloc(sizeof(t_philo) * program->nb_philos);
	if (!program->forks || !program->philos)
		return (ft_error(), 1);
	pthread_mutex_init(&program->write_lock, NULL);
	pthread_mutex_init(&program->dead_lock, NULL);
	init_philo(program);
	return (0);
}

int	is_simulation_over(t_program *program)
{
	int	result;

	pthread_mutex_lock(&program->dead_lock);
	result = program->dead_flag;
	pthread_mutex_unlock(&program->dead_lock);
	return (result);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (!is_simulation_over(philo->program)) // Vérification constante
	{
		if (is_simulation_over(philo->program)) // Sortir si la simulation est terminée
			break ;

		pthread_mutex_lock(philo->r_fork);
		if (is_simulation_over(philo->program)) // Sortir si la simulation est terminée
			return (pthread_mutex_unlock(philo->r_fork), NULL);
		pthread_mutex_lock(&philo->program->write_lock);
		printf("%zu %d has taken a fork\n", get_current_time() - philo->program->start_time, philo->id);
		pthread_mutex_unlock(&philo->program->write_lock);

		pthread_mutex_lock(philo->l_fork);
		if (is_simulation_over(philo->program)) // Sortir si la simulation est terminée
			return (pthread_mutex_unlock(philo->r_fork), pthread_mutex_unlock(philo->l_fork), NULL);
		pthread_mutex_lock(&philo->program->write_lock);
		printf("%zu %d has taken a fork\n", get_current_time() - philo->program->start_time, philo->id);
		printf("%zu %d is eating\n", get_current_time() - philo->program->start_time, philo->id);
		pthread_mutex_unlock(&philo->program->write_lock);

		// Mise à jour de `last_meal`
		pthread_mutex_lock(&philo->program->dead_lock);
		philo->last_meal = get_current_time();
		pthread_mutex_unlock(&philo->program->dead_lock);

		ft_usleep(philo->program->time_to_eat);
		philo->meals_eaten++;

		pthread_mutex_unlock(philo->l_fork);
		pthread_mutex_unlock(philo->r_fork);

		pthread_mutex_lock(&philo->program->write_lock);
		printf("%zu %d is sleeping\n", get_current_time() - philo->program->start_time, philo->id);
		pthread_mutex_unlock(&philo->program->write_lock);
		ft_usleep(philo->program->time_to_sleep);

		pthread_mutex_lock(&philo->program->write_lock);
		printf("%zu %d is thinking\n", get_current_time() - philo->program->start_time, philo->id);
		pthread_mutex_unlock(&philo->program->write_lock);
	}
	return (NULL);
}


void	*game_master(void *arg)
{
	t_program	*program;
	int			i;
	int			all_ate;

	program = (t_program *)arg;
	while (1)
	{
		i = 0;
		all_ate = 1;
		while (i < program->nb_philos)
		{
			pthread_mutex_lock(&program->dead_lock);
			if ((get_current_time() - program->philos[i].last_meal) > (size_t)program->time_to_die)
			{
				if (!program->dead_flag)
				{
					program->dead_flag = 1;
					pthread_mutex_lock(&program->write_lock);
					printf("%zu %d died\n", get_current_time() - program->start_time, program->philos[i].id);
					pthread_mutex_unlock(&program->write_lock);
				}
				pthread_mutex_unlock(&program->dead_lock);
				return (NULL);
			}
			if (program->philos[i].meals_eaten < program->max_meals)
				all_ate = 0;
			pthread_mutex_unlock(&program->dead_lock);
			i++;
		}

		if (all_ate)
		{
			pthread_mutex_lock(&program->write_lock);
			printf("All philosophers have eaten the maximum number of meals\n");
			pthread_mutex_unlock(&program->write_lock);
			program->dead_flag = 1;
			return (NULL);
		}

		usleep(500);
	}
	return (NULL);
}


int	main(int ac, char **av)
{
	t_program	program;
	pthread_t	master;
	int			i;

	if (!(ac == 5 || ac == 6))
		return (ft_error(), 1);
	if (!ft_parsing(ac, av))
		return (1);
	if (init_program(&program, ac, av))
		return (1);
	i = -1;
	while (++i < program.nb_philos)
		pthread_create(&program.philos[i].thread, NULL, philo_routine, &program.philos[i]);
	pthread_create(&master, NULL, game_master, &program);
	pthread_join(master, NULL);
	i = -1;
	while (++i < program.nb_philos)
		pthread_join(program.philos[i].thread, NULL);
	free(program.forks);
	free(program.philos);
	return (0);
}
