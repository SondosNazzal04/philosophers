/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snazzal <snazzal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 17:33:54 by snazzal           #+#    #+#             */
/*   Updated: 2025/06/26 17:43:01 by snazzal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	eating(t_philo *philo)
{
	t_forks	*first;
	t_forks	*second;

	if (check_dead(philo->data))
		return (0);
	setup_forks(philo, &first, &second);
	if (!take_forks(philo, first, second))
		return (0);
	update_meal_state(philo, 1);
	print_state("is eating 🍝", philo);
	if (!inter_milisleep(philo->data, philo->data->time_to_eat))
	{
		update_meal_state(philo, 0);
		pthread_mutex_unlock(&first->mutex);
		pthread_mutex_unlock(&second->mutex);
		return (0);
	}
	update_meal_state(philo, 0);
	pthread_mutex_unlock(&first->mutex);
	pthread_mutex_unlock(&second->mutex);
	return (1);
}

int	thinking(t_philo *philo)
{
	if (check_dead(philo->data))
		return (0);
	print_state("is thinking 🤔", philo);
	usleep(500);
	return (1);
}

int	sleeping(t_philo *philo)
{
	if (check_dead(philo->data))
		return (0);
	print_state("is sleeping 💤", philo);
	if (!inter_milisleep(philo->data, philo->data->time_to_sleep))
		return (0);
	return (1);
}

void	*routine_philo(void *args)
{
	t_philo	*philo;

	philo = (t_philo *)args;
	wait_threads(philo);
	if (philo->id % 2 == 0)
		usleep(1000);
	while (1)
	{
		if (check_dead(philo->data))
			break ;
		if (done_eating(philo))
			break ;
		if (!eating(philo))
			break ;
		if (!thinking(philo))
			break ;
		if (!sleeping(philo))
			break ;
	}
	return (NULL);
}
