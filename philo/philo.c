/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 16:10:57 by halzamma          #+#    #+#             */
/*   Updated: 2025/05/24 14:31:29 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*one_philo_routine(void *arg)
{
	t_philo *philo;

	philo = (t_philo *)arg;
	print_action(philo, "is thinking");
	pthread_mutex_lock(philo->left_fork);
	print_action(philo, "has taken a fork");
	smart_sleep(philo->data->time_to_die, philo->data);
	pthread_mutex_lock(&philo->data->print_mutex);
	printf("%ld philo %d died\n", get_time() - philo->data->start_time, philo->id);
	pthread_mutex_unlock(&philo->data->print_mutex);
	pthread_mutex_unlock(philo->left_fork);
	return (NULL);
}


void	*philo_routine(void *arg)
{
	t_philo			*philo = (t_philo *)arg;
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;
	int				is_running;

	if (philo->id % 2 == 0)
		usleep(1000);

	while (1)
	{
		pthread_mutex_lock(&philo->data->death_mutex);
		is_running = (!philo->data->dead && !philo->data->all_ate);
		pthread_mutex_unlock(&philo->data->death_mutex);
		if (!is_running)
			break;

		print_action(philo, "is thinking");

		get_ordered_forks(philo, &first, &second);
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
			break ;
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
	}
	return (NULL);
}

void	start_simulation(t_data *data)
{
	int i;
	pthread_t monitor_thread;

	i = 0;
	if (data->num_philos == 1)
	{
		pthread_create(&data->philos[0].thread, NULL, one_philo_routine, &data->philos[0]);
		pthread_join(data->philos[0].thread, NULL);
		return ;
	}
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
