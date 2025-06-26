/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snazzal <snazzal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 17:50:48 by snazzal           #+#    #+#             */
/*   Updated: 2025/06/26 17:54:44 by snazzal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	wait_threads_m(t_data *data)
{
	pthread_mutex_lock(&data->state);
	while (!data->ready)
	{
		pthread_mutex_unlock(&data->state);
		usleep(100);
		pthread_mutex_lock(&data->state);
	}
	pthread_mutex_unlock(&data->state);
}

void	*monitor(void *args)
{
	t_data	*data;

	data = (t_data *)args;
	wait_threads_m(data);
	while (1)
	{
		if (data->meals_required != -1)
		{
			if (has_eaten_enough(data))
			{
				pthread_mutex_lock(&data->print);
				pthread_mutex_lock(&data->state);
				data->is_stopped = 1;
				pthread_mutex_unlock(&data->state);
				pthread_mutex_unlock(&data->print);
				return (NULL);
			}
		}
		if (check_all_death(data))
			break ;
		usleep(1000);
	}
	return (NULL);
}
