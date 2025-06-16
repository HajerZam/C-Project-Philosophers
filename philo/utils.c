/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 16:12:33 by halzamma          #+#    #+#             */
/*   Updated: 2025/06/14 15:13:34 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	smart_sleep(long time_in_ms, t_data *data)
{
	long	start;
	long	now;
	long	sleep_time;

	start = get_time();
	while (1)
	{
		pthread_mutex_lock(&data->death_mutex);
		if (data->dead || data->all_ate)
		{
			pthread_mutex_unlock(&data->death_mutex);
			break ;
		}
		pthread_mutex_unlock(&data->death_mutex);
		now = get_time();
		if ((now - start) >= time_in_ms)
			break ;
		sleep_time = time_in_ms - (now - start);
		if (sleep_time > 10)
			usleep(1000);
		else
			usleep(100);
	}
}

void	print_action(t_philo *philo, const char *action)
{
	long	timestamp;

	pthread_mutex_lock(&philo->data->print_mutex);
	pthread_mutex_lock(&philo->data->death_mutex);
	if (!philo->data->dead && !philo->data->all_ate)
	{
		timestamp = get_time() - philo->data->start_time;
		printf("%ld philo %d %s\n", timestamp, philo->id, action);
	}
	pthread_mutex_unlock(&philo->data->death_mutex);
	pthread_mutex_unlock(&philo->data->print_mutex);
}
