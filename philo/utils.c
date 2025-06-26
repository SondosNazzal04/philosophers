/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snazzal <snazzal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 12:09:40 by snazzal           #+#    #+#             */
/*   Updated: 2025/06/26 19:36:28 by snazzal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *nptr)
{
	int	num;
	int	count;
	int	i;

	count = 0;
	i = 0;
	num = 0;
	while (nptr[i] == ' ' || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '-' || nptr[i] == '+' )
	{
		if (nptr[i] == '-')
			count++;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		num = num * 10 + nptr[i] - 48;
		i++;
	}
	if (count % 2 == 1)
		num = -1 * num;
	return (num);
}

void	print_state(char *string, t_philo *philo)
{
	long long	timestamp;

	if (check_dead(philo->data))
		return ;
	timestamp = get_time() - philo->data->start_time;
	pthread_mutex_lock(&philo->data->print);
	printf("%lld %d %s\n",
		timestamp, philo->id, string);
	pthread_mutex_unlock(&philo->data->print);
}

void	clean_up(t_data *data)
{
	if (data->forks)
	{
		free(data->forks);
		data->forks = NULL;
	}
	if (data->philos)
	{
		free(data->philos);
		data->philos = NULL;
	}
}

int	join_threads(t_data *data)
{
	int	counter;

	counter = 0;
	while (counter < data->n_philo)
	{
		if (pthread_join(data->philos[counter].philo_id, NULL) != 0)
			return (0);
		counter++;
	}
	if (pthread_join(data->monitor, NULL) != 0)
		return (0);
	return (1);
}

void	update_meal_state(t_philo *philo, int flag)
{
	pthread_mutex_lock(&philo->eating);
	if (flag)
	{
		philo->is_eating = 1;
		philo->last_meal = get_time();
	}
	else
	{
		philo->is_eating = 0;
		philo->meals_eaten++;
	}
	pthread_mutex_unlock(&philo->eating);
}
