/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 16:10:57 by halzamma          #+#    #+#             */
/*   Updated: 2025/06/10 15:08:42 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

static int	philo_check_and_lock_first(t_philo *philo, pthread_mutex_t **first)
{
	get_ordered_forks(philo, first, NULL);
	pthread_mutex_lock(*first);
	print_action(philo, "has taken a fork");
	pthread_mutex_lock(&philo->data->death_mutex);
	if (philo->data->dead)
	{
		pthread_mutex_unlock(*first);
		pthread_mutex_unlock(&philo->data->death_mutex);
		return (0);
	}
	pthread_mutex_unlock(&philo->data->death_mutex);
	return (1);
}

static int	philo_check_and_lock_second(t_philo *philo,
	pthread_mutex_t *first, pthread_mutex_t **second)
{
	get_ordered_forks(philo, NULL, second);
	pthread_mutex_lock(*second);
	print_action(philo, "has taken a fork");
	pthread_mutex_lock(&philo->data->death_mutex);
	if (philo->data->dead)
	{
		pthread_mutex_unlock(*second);
		pthread_mutex_unlock(first);
		pthread_mutex_unlock(&philo->data->death_mutex);
		return (0);
	}
	pthread_mutex_unlock(&philo->data->death_mutex);
	return (1);
}

static void	eating_meal(
	t_philo *philo, pthread_mutex_t *first, pthread_mutex_t *second)
{
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->data->death_mutex);
	print_action(philo, "is eating");
	smart_sleep(philo->data->time_to_eat, philo->data);
	philo->meals_eaten++;
	pthread_mutex_unlock(second);
	pthread_mutex_unlock(first);
	print_action(philo, "is sleeping");
	smart_sleep(philo->data->time_to_sleep, philo->data);
}

void	*philo_routine(void *arg)
{
	t_philo			*philo;
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	philo = (t_philo *)arg;
	if (philo->data->num_philos > 2)
		smart_sleep((philo->id - 1) * 20, philo->data);
	while (run_and_think(philo))
	{
		pthread_mutex_lock(&philo->data->death_mutex);
		if (philo->data->dead)
		{
			pthread_mutex_unlock(&philo->data->death_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->data->death_mutex);
		if (!philo_check_and_lock_first(philo, &first))
			break ;
		if (!philo_check_and_lock_second(philo, first, &second))
			break ;
		eating_meal(philo, first, second);
	}
	return (NULL);
}
