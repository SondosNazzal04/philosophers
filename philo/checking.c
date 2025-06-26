/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checking.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snazzal <snazzal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 17:39:32 by snazzal           #+#    #+#             */
/*   Updated: 2025/06/26 17:42:15 by snazzal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	done_eating(t_philo *philo)
{
	int	done_eating;

	done_eating = 0;
	if (philo->data->meals_required == -1)
		return (0);
	pthread_mutex_lock(&philo->eating);
	done_eating = (philo->meals_eaten >= philo->data->meals_required);
	pthread_mutex_unlock(&philo->eating);
	return (done_eating);
}

int	check_dead(t_data *data)
{
	int	stop;

	stop = 0;
	pthread_mutex_lock(&data->state);
	stop = data->is_stopped;
	pthread_mutex_unlock(&data->state);
	return (stop);
}

int	check_all_death(t_data *data)
{
	int	counter;

	counter = 0;
	while (counter < data->n_philo)
	{
		if (check_death(&data->philos[counter]) == 1)
			return (1);
		counter++;
	}
	return (0);
}

int	check_death(t_philo *philo)
{
	long long	time;
	int			dead;

	dead = 0;
	pthread_mutex_lock(&philo->eating);
	time = get_time();
	dead = (!philo->is_eating
			&& (time - philo->last_meal) >= philo->data->time_to_die);
	if (dead)
	{
		pthread_mutex_unlock(&philo->eating);
		pthread_mutex_lock(&philo->data->print);
		pthread_mutex_lock(&philo->data->state);
		philo->data->is_stopped = 1;
		printf("%lld %d died 💀\n", (time - philo->data->start_time), philo->id);
		pthread_mutex_unlock(&philo->data->state);
		pthread_mutex_unlock(&philo->data->print);
		return (1);
	}
	pthread_mutex_unlock(&philo->eating);
	return (0);
}

int	has_eaten_enough(t_data *data)
{
	int	counter;
	int	has_eaten_enough;

	counter = 0;
	has_eaten_enough = 1;
	while (counter < data->n_philo)
	{
		if (!done_eating(&data->philos[counter]))
		{
			has_eaten_enough = 0;
			break ;
		}
		counter++;
	}
	return (has_eaten_enough);
}
