/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halzamma <halzamma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 14:21:16 by halzamma          #+#    #+#             */
/*   Updated: 2025/06/14 13:19:46 by halzamma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	get_ordered_forks(
	t_philo *philo, pthread_mutex_t **first, pthread_mutex_t **second)
{
	if (philo->id % 2 == 0)
	{
		*first = philo->right_fork;
		*second = philo->left_fork;
	}
	else
	{
		*first = philo->left_fork;
		*second = philo->right_fork;
	}
}

static int	check_and_unlock(t_philo *philo, pthread_mutex_t *to_unlock)
{
	int	dead;

	pthread_mutex_lock(&philo->data->death_mutex);
	dead = philo->data->dead;
	pthread_mutex_unlock(&philo->data->death_mutex);
	if (dead)
	{
		pthread_mutex_unlock(to_unlock);
		return (1);
	}
	return (0);
}

int	take_forks_and_eat(t_philo *philo)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	get_ordered_forks(philo, &first, &second);
	pthread_mutex_lock(first);
	print_action(philo, "has taken a fork");
	if (check_and_unlock(philo, first))
		return (0);
	pthread_mutex_lock(second);
	print_action(philo, "has taken a fork");
	if (check_and_unlock(philo, second))
	{
		pthread_mutex_unlock(first);
		return (0);
	}
	eating_meal(philo, first, second);
	return (1);
}
