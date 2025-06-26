/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snazzal <snazzal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 17:45:00 by snazzal           #+#    #+#             */
/*   Updated: 2025/06/26 17:52:31 by snazzal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	inter_milisleep(t_data *data, long long time)
{
	long long	current_time;

	current_time = get_time();
	while ((get_time() - current_time) < time)
	{
		if (check_dead(data))
			return (0);
		usleep(100);
	}
	return (1);
}

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	wait_threads(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->state);
	while (philo->data->ready == 0)
	{
		pthread_mutex_unlock(&philo->data->state);
		usleep(100);
		pthread_mutex_lock(&philo->data->state);
	}
	pthread_mutex_unlock(&philo->data->state);
}
