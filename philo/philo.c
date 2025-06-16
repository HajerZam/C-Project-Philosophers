/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 16:10:57 by halzamma          #+#    #+#             */
/*   Updated: 2025/06/16 14:49:25 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*one_philo_routine(void *arg)
{
	t_philo	*philo;
	long	death_time;

	philo = (t_philo *)arg;
	print_action(philo, "is thinking");
	pthread_mutex_lock(philo->left_fork);
	print_action(philo, "has taken a fork");
	death_time = philo->data->start_time + philo->data->time_to_die;
	while (get_time() < death_time)
		usleep(1000);
	pthread_mutex_lock(&philo->data->print_mutex);
	printf("%ld %d died\n", get_time() - philo->data->start_time, philo->id);
	pthread_mutex_unlock(&philo->data->print_mutex);
	pthread_mutex_unlock(philo->left_fork);
	return (NULL);
}

void	philo_take_forks(t_philo *philo,
	pthread_mutex_t *first, pthread_mutex_t *second)
{
	get_ordered_forks(philo, &first, &second);
	pthread_mutex_lock(first);
	print_action(philo, "has taken a fork");
	pthread_mutex_lock(second);
	print_action(philo, "has taken a fork");
}

static int	run_and_think(t_philo *philo)
{
	int	is_running;

	pthread_mutex_lock(&philo->data->death_mutex);
	is_running = (!philo->data->dead && !philo->data->all_ate);
	pthread_mutex_unlock(&philo->data->death_mutex);
	if (!is_running)
		return (0);
	print_action(philo, "is thinking");
	return (1);
}

void	eating_meal(
	t_philo *philo, pthread_mutex_t *first, pthread_mutex_t *second)
{
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->meal_mutex);
	print_action(philo, "is eating");
	smart_sleep(philo->data->time_to_eat, philo->data);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_mutex);
	pthread_mutex_unlock(second);
	pthread_mutex_unlock(first);
	print_action(philo, "is sleeping");
	smart_sleep(philo->data->time_to_sleep, philo->data);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(100);
	while (1)
	{
		if (!run_and_think(philo))
			break ;
		pthread_mutex_lock(&philo->data->death_mutex);
		if (philo->data->dead)
		{
			pthread_mutex_unlock(&philo->data->death_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->data->death_mutex);
		if (!take_forks_and_eat(philo))
			break ;
		usleep(1000);
	}
	return (NULL);
}
