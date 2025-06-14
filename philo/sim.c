/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 14:45:19 by halzamma          #+#    #+#             */
/*   Updated: 2025/06/14 14:03:13 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	create_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_create(&data->philos[i].thread, NULL,
				philo_routine, &data->philos[i]) != 0)
		{
			print_error("Failed to create thread");
			return (1);
		}
		i++;
	}
	return (0);
}

static void	join_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
}

void	start_simulation(t_data *data)
{
	pthread_t	monitor_thread;

	if (data->num_philos == 1)
	{
		if (pthread_create(&data->philos[0].thread, NULL,
				one_philo_routine, &data->philos[0]) != 0)
		{
			print_error("Failed to create thread");
			return ;
		}
		pthread_join(data->philos[0].thread, NULL);
		return ;
	}
	if (create_threads(data))
		return ;
	if (pthread_create(&monitor_thread, NULL,
			monitor_routine, data) != 0)
	{
		print_error("Failed to create monitor thread");
		return ;
	}
	pthread_join(monitor_thread, NULL);
	join_threads(data);
}
