/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 14:05:05 by halzamma          #+#    #+#             */
/*   Updated: 2025/05/24 14:33:32 by halzamma         ###   ########.fr       */
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


void	*monitor_routine(void *arg)
{
	t_data	*data = (t_data *)arg;
	int		i;
	long	now;
	long	last_meal;

	while (1)
	{
		now = get_time(); // Get time once per outer loop
		i = 0;
		while (i < data->num_philos)
		{
			pthread_mutex_lock(&data->death_mutex);
			last_meal = data->philos[i].last_meal;
			if ((now - last_meal) > data->time_to_die)
			{
				data->dead = 1;
				pthread_mutex_unlock(&data->death_mutex);

				pthread_mutex_lock(&data->print_mutex);
				printf("%ld philo %d died\n", now - data->start_time, data->philos[i].id);
				pthread_mutex_unlock(&data->print_mutex);
				return (NULL);
			}

			if (data->meals_required > 0 && check_all_ate(data))
			{
				data->all_ate = 1;
				pthread_mutex_unlock(&data->death_mutex);
				return (NULL);
			}
			pthread_mutex_unlock(&data->death_mutex);

			i++;
		}
		usleep(1000); // avoid CPU spinning
	}
}
