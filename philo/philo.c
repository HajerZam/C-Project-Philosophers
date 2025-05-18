/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 16:10:57 by halzamma          #+#    #+#             */
/*   Updated: 2025/05/18 14:56:07 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philo_routine(void *arg)
{
	t_philo	*philo = (t_philo *)arg;
	pthread_mutex_t *first;
	pthread_mutex_t *second;
	int		is_running;

	if (philo->id % 2 == 0)
		usleep(1000);
	pthread_mutex_lock(&philo->data->death_mutex);
	is_running = (!philo->data->dead && !philo->data->all_ate);
	pthread_mutex_unlock(&philo->data->death_mutex);
	while (is_running)
	{
		print_action(philo, "is thinking");
		if (philo->left_fork < philo->right_fork)
		{
			first = philo->left_fork;
			second = philo->right_fork;
		}
		else
		{
			first = philo->right_fork;
			second = philo->left_fork;
		}
		pthread_mutex_lock(first);
		print_action(philo, "has taken a fork");
		pthread_mutex_lock(second);
		print_action(philo, "has taken a fork");
		pthread_mutex_lock(&philo->data->death_mutex);
		if (philo->data->dead || philo->data->all_ate)
		{
			pthread_mutex_unlock(second);
			pthread_mutex_unlock(first);
			pthread_mutex_unlock(&philo->data->death_mutex);
			return (NULL);
		}
		philo->last_meal = get_time();
		pthread_mutex_unlock(&philo->data->death_mutex);
		print_action(philo, "is eating");
		smart_sleep(philo->data->time_to_eat, philo->data);
		philo->meals_eaten++;
		pthread_mutex_unlock(second);
		pthread_mutex_unlock(first);
		print_action(philo, "is sleeping");
		smart_sleep(philo->data->time_to_sleep, philo->data);
		// looping condition
		pthread_mutex_lock(&philo->data->death_mutex);
		is_running = (!philo->data->dead && !philo->data->all_ate);
		pthread_mutex_unlock(&philo->data->death_mutex);
	}
	return (NULL);
}

void	start_simulation(t_data *data)
{
	int			i;
	pthread_t   monitor_thread;

	i = 0;
	while (i < data->num_philos)
	{
		data->philos[i].last_meal = get_time();
		if (pthread_create(&data->philos[i].thread, NULL,
			philo_routine, &data->philos[i]) != 0)
		{
			print_error("Failed to create thread");
			return ;
		}
		i++;
	}
	if (pthread_create(&monitor_thread, NULL, monitor_routine, data) != 0)
	{
		print_error("Failed to create monitor thread");
		return ;
	}
	pthread_detach(monitor_thread);
	i = 0;
	while (i < data->num_philos)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
}
