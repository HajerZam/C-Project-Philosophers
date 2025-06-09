/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 14:05:05 by halzamma          #+#    #+#             */
/*   Updated: 2025/06/03 16:27:20 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_all_ate(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		if (data->philos[i].meals_eaten < data->meals_required)
			return (0);
		i++;
	}
	return (1);
}

static int	check_death(t_data *data, int i, long now)
{
	long	last_meal;
	long	time_since_meal;

	pthread_mutex_lock(&data->death_mutex);
	if (data->dead)
	{
		pthread_mutex_unlock(&data->death_mutex);
		return (1);
	}
	pthread_mutex_lock(&data->philos[i].meal_mutex);
	last_meal = data->philos[i].last_meal;
	pthread_mutex_unlock(&data->philos[i].meal_mutex);
	time_since_meal = now - last_meal;
	if (time_since_meal > data->time_to_die)
	{
		data->dead = 1;
		pthread_mutex_unlock(&data->death_mutex);
		pthread_mutex_lock(&data->print_mutex);
		printf("%ld %d died\n", now - data->start_time, data->philos[i].id);
		pthread_mutex_unlock(&data->print_mutex);
		return (1);
	}
	pthread_mutex_unlock(&data->death_mutex);
	return (0);
}

static int	check_meals(t_data *data)
{
	pthread_mutex_lock(&data->death_mutex);
	if (data->dead || (data->meals_required > 0 && check_all_ate(data)))
	{
		data->all_ate = 1;
		pthread_mutex_unlock(&data->death_mutex);
		return (1);
	}
	pthread_mutex_unlock(&data->death_mutex);
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		i;
	long	now;

	data = (t_data *)arg;
	while (1)
	{
		now = get_time();
		i = 0;
		while (i < data->num_philos)
		{
			if (check_death(data, i, now))
				return (NULL);
			if (check_meals(data))
				return (NULL);
			i++;
		}
		usleep(1000);
	}
}
