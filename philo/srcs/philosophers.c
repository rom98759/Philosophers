/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcaillie <rcaillie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 13:38:05 by rcaillie          #+#    #+#             */
/*   Updated: 2024/12/17 12:31:01 by rcaillie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_philo(t_program *program)
{
	int	i;

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
	int	i;

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
	i = -1;
	while (++i < program->nb_philos)
		pthread_mutex_init(&program->forks[i], NULL);
	init_philo(program);
	return (0);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while ((philo->program->max_meals != -1 && philo->meals_eaten < philo->program->max_meals) || !philo->program->dead_flag)
	{
		// Printf en rouge
		// printf("\033[0;31mPhilo %d number of meals eaten: %d\n\033[0m", philo->id, philo->meals_eaten);

		printf("%zu %d is thinking\n", get_current_time(), philo->id);
		pthread_mutex_lock(philo->r_fork);
		printf("%zu %d has taken a fork\n", get_current_time(), philo->id);
		pthread_mutex_lock(philo->l_fork);
		printf("%zu %d is eating\n", get_current_time(), philo->id);
		ft_usleep(philo->program->time_to_eat);
		philo->last_meal = get_current_time();
		philo->meals_eaten++;
		pthread_mutex_unlock(philo->l_fork);
		pthread_mutex_unlock(philo->r_fork);
		printf("%zu %d is sleeping\n", get_current_time(), philo->id);
		ft_usleep(philo->program->time_to_sleep);
	}
	printf("\033[0;32m%zu %d is dead\n\033[0m", get_current_time(), philo->id);
	return (NULL);
}

int	main(int ac, char **av)
{
	t_program	program;
	int			i;

	if (!(ac == 5 || ac == 6))
		return (ft_error(), 1);
	if (!ft_parsing(ac, av))
		return (1);
	if (init_program(&program, ac, av))
		return (1);
	i = -1;
	while (++i < program.nb_philos)
		pthread_create(&program.philos[i].thread, NULL, routine, &program.philos[i]);
	i = -1;
	while (++i < program.nb_philos)
		pthread_join(program.philos[i].thread, NULL);
	free(program.forks);
	free(program.philos);
	return (0);
}
