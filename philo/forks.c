/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snazzal <snazzal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 13:36:01 by snazzal           #+#    #+#             */
/*   Updated: 2025/06/26 17:46:36 by snazzal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_forks(t_data *data)
{
	int		counter;
	t_forks	*forks;

	counter = 0;
	forks = malloc(sizeof(t_forks) * data->n_philo);
	if (forks == NULL)
		return (0);
	memset(forks, 0, sizeof(t_forks) * data->n_philo);
	while (counter < data->n_philo)
	{
		forks[counter].id = counter;
		forks[counter].mutex = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
		counter++;
	}
	data->ready = 0;
	data->forks = forks;
	return (1);
}

void	setup_forks(t_philo *philo, t_forks **first, t_forks **second)
{
	if (philo->id % 2 == 0)
	{
		*first = philo->right;
		*second = philo->left;
	}
	else
	{
		*first = philo->left;
		*second = philo->right;
	}
}

int	take_forks(t_philo *philo, t_forks *first, t_forks *second)
{
	pthread_mutex_lock(&first->mutex);
	if (check_dead(philo->data))
	{
		pthread_mutex_unlock(&first->mutex);
		return (0);
	}
	print_state("has taken a fork 🍴", philo);
	if (philo->data->n_philo == 1)
	{
		pthread_mutex_unlock(&first->mutex);
		inter_milisleep(philo->data, philo->data->time_to_die);
		return (0);
	}
	pthread_mutex_lock(&second->mutex);
	if (check_dead(philo->data))
	{
		pthread_mutex_unlock(&first->mutex);
		pthread_mutex_unlock(&second->mutex);
		return (0);
	}
	print_state("has taken a fork 🍴", philo);
	return (1);
}
