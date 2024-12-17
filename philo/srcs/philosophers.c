/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcaillie <rcaillie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 13:38:05 by rcaillie          #+#    #+#             */
/*   Updated: 2024/12/17 14:36:32 by rcaillie         ###   ########.fr       */
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

void	print_action(t_philo *philo, char *action)
{
	pthread_mutex_lock(&philo->program->write_lock);
	if (!is_simulation_over(philo->program))
		printf("%zu %d %s\n", get_current_time() - philo->program->start_time, philo->id, action);
	pthread_mutex_unlock(&philo->program->write_lock);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->program->nb_philos == 1)
	{
		print_action(philo, "has taken a fork");
		ft_usleep(philo->program->time_to_die);
		pthread_mutex_lock(&philo->program->write_lock);
		pthread_mutex_unlock(&philo->program->write_lock);
		return (NULL);
	}
	if (philo->id % 2 == 0) // Philosophe pair
		usleep(50); // Décalage pour éviter que tout le monde prenne les fourchettes en même temps
	while (!is_simulation_over(philo->program))
	{
		// Prise des fourchettes
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
		print_action(philo, "is eating");

		// Mise à jour du temps et des repas
		pthread_mutex_lock(&philo->program->dead_lock);
		philo->last_meal = get_current_time();
		philo->meals_eaten++;
		pthread_mutex_unlock(&philo->program->dead_lock);

		ft_usleep(philo->program->time_to_eat);

		pthread_mutex_unlock(philo->l_fork);
		pthread_mutex_unlock(philo->r_fork);

		// Philosophe dort
		print_action(philo, "is sleeping");
		ft_usleep(philo->program->time_to_sleep);

		// Philosophe pense
		print_action(philo, "is thinking");
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
				program->dead_flag = 1;
				pthread_mutex_lock(&program->write_lock);
				printf("%zu %d died\n", get_current_time() - program->start_time, program->philos[i].id);
				pthread_mutex_unlock(&program->write_lock);
				pthread_mutex_unlock(&program->dead_lock);
				return (NULL);
			}
			if (program->philos[i].meals_eaten < program->max_meals || program->max_meals == -1)
				all_ate = 0;
			pthread_mutex_unlock(&program->dead_lock);
			i++;
		}
		if (all_ate)
		{
			pthread_mutex_lock(&program->write_lock);
			printf("All philosophers have eaten the maximum number of meals\n");
			pthread_mutex_unlock(&program->write_lock);
			pthread_mutex_lock(&program->dead_lock);
			program->dead_flag = 1;
			pthread_mutex_unlock(&program->dead_lock);
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
	{
		pthread_create(&program.philos[i].thread, NULL, philo_routine, &program.philos[i]);
		if (program.nb_philos > 1)
			usleep(1);
	}
	pthread_create(&master, NULL, game_master, &program);
	pthread_join(master, NULL);
	i = -1;
	while (++i < program.nb_philos)
		pthread_join(program.philos[i].thread, NULL);
	free(program.forks);
	free(program.philos);
	return (0);
}
