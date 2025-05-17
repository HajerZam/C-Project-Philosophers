/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 14:05:05 by halzamma          #+#    #+#             */
/*   Updated: 2025/05/09 14:05:05 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_all_ate(t_data *data)
{
	int	i;
	int	done;

	done = 1;
	i = 0;
	while(i < data->num_philos)
	{
		if (data->philos[i].meals_eaten < data->meals_required)
		{
			done = 0;
			break;
		}
		i++;
	}
	return (done);
}

void	*monitor_routine(void *arg)
{
	t_data	*data = (t_data *)arg;
	int		i;
	long	now;

	while (1)
	{
		i = 0;
		while (i < data->num_philos)
		{
			pthread_mutex_lock(&data->death_mutex);
			now = get_time();
			if ((now - data->philos[i].last_meal) > data->time_to_die)
			{
				data->dead = 1;
				pthread_mutex_lock(&data->print_mutex);
				printf("%ld %d died\n", now - data->start_time, data->philos[i].id);
				pthread_mutex_unlock(&data->print_mutex);
				pthread_mutex_unlock(&data->death_mutex);
				return (NULL);
			}
			pthread_mutex_unlock(&data->death_mutex);
			i++;
		}
		pthread_mutex_lock(&data->death_mutex);
		if (data->dead)
		{
			pthread_mutex_unlock(&data->death_mutex);
			return (NULL);
		}
		if (data->meals_required > 0 && check_all_ate(data))
		{
			data->all_ate = 1;
			return (NULL);
		}
		usleep(1000);
	}
}
