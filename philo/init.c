/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snazzal <snazzal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 17:48:11 by snazzal           #+#    #+#             */
/*   Updated: 2025/06/26 18:02:25 by snazzal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	parse_args(t_data *data, int argc, char **argv)
{
	if (argc < 5 || argc > 6)
	{
		write(2, "Invalid Input\n", 15);
		return (0);
	}
	data->n_philo = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->meals_required = -1;
	if (argc == 6)
		data->meals_required = ft_atoi(argv[5]);
	if (data->n_philo <= 0 || data->time_to_die <= 0 || data->time_to_eat <= 0
		|| data->time_to_sleep <= 0 || (argc == 6 && data->meals_required <= 0))
	{
		write(2, "Invalid Input\n", 15);
		return (0);
	}
	data->print = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
	data->state = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
	return (1);
}

int	philo_init(t_data *data)
{
	t_philo	*philo;
	int		counter;

	counter = 0;
	philo = malloc(data->n_philo * sizeof(t_philo));
	if (philo == NULL)
		return (0);
	memset(philo, 0, sizeof(t_philo) * data->n_philo);
	data->start_time = get_time();
	while (counter < data->n_philo)
	{
		philo[counter].data = data;
		philo[counter].id = counter + 1;
		philo[counter].right = &data->forks[counter];
		philo[counter].left = &data->forks[(counter + 1) % data->n_philo];
		philo[counter].is_eating = 0;
		philo[counter].last_meal = data->start_time;
		philo[counter].eating = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
		counter++;
	}
	data->philos = philo;
	return (1);
}

int	init_threads(t_data *data)
{
	int	counter;

	counter = 0;
	while (counter < data->n_philo)
	{
		if (pthread_create(&data->philos[counter].philo_id, NULL,
				routine_philo, &data->philos[counter]) != 0)
			return (0);
		counter++;
	}
	pthread_mutex_lock(&data->state);
	data->ready = 1;
	pthread_mutex_unlock(&data->state);
	if (pthread_create(&data->monitor, NULL, monitor, data) != 0)
		return (0);
	if (!join_threads(data))
		return (0);
	return (1);
}
