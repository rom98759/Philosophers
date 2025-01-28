/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcaillie <rcaillie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 13:38:42 by rcaillie          #+#    #+#             */
/*   Updated: 2025/01/28 14:05:00 by rcaillie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_philo
{
	int					id;
	int					meals_eaten;
	size_t				last_meal;
	pthread_t			thread;
	struct s_program	*program;
	pthread_mutex_t		*r_fork;
	pthread_mutex_t		*l_fork;
}						t_philo;

typedef struct s_program
{
	int				nb_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				max_meals;
	int				dead_flag;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	dead_lock;
	pthread_mutex_t	*forks;
	t_philo			*philos;
	unsigned long	start_time;
}					t_program;

/* Philosophers */
void	end_simulation(t_program *program);

/* Threads utils */
int		is_simulation_over(t_program *program);
void	print_action(t_philo *philo, char *action);
void	take_forks(t_philo *philo);
void	update_philo_state(t_philo *philo);

/* Utils */
long	ft_atol(const char *str);
long	get_current_time(void);
void	ft_usleep(size_t milliseconds);
void	ft_error(void);

/* Parsing */
int		ft_parsing(int ac, char **av);

/* Threads */
void	*philo_routine(void *arg);
void	*game_master(void *arg);

/* Init */
int		init_program(t_program *program, int ac, char **av);

#endif
