/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 16:11:08 by halzamma          #+#    #+#             */
/*   Updated: 2025/06/14 13:23:22 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <string.h>
# include <limits.h>

typedef struct s_data
{
	int				num_philos;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				meals_required;
	int				all_ate;
	int				dead;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	death_mutex;
	long			start_time;
	struct s_philo	*philos;
}	t_data;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long			last_meal;
	pthread_t		thread;
	t_data			*data;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	meal_mutex;
}	t_philo;

int		init_all(t_data *data, int argc, char **argv);
int		is_number(char *str);
int		ft_isdigit(char c);
void	*philo_routine(void *arg);
void	start_simulation(t_data *data);
void	eating_meal(
			t_philo *philo,
			pthread_mutex_t *first,
			pthread_mutex_t *second);

void	get_ordered_forks(
			t_philo *philo,
			pthread_mutex_t **first,
			pthread_mutex_t **second);

void	philo_take_forks(
			t_philo *philo,
			pthread_mutex_t *first,
			pthread_mutex_t *second);

void	cleanup(t_data *data);
int		take_forks_and_eat(t_philo *philo);
long	get_time(void);
void	smart_sleep(long duration, t_data *data);
void	print_action(t_philo *philo, const char *action);
int		print_error(const char *msg);
void	*one_philo_routine(void *arg);
void	*monitor_routine(void *arg);

#endif