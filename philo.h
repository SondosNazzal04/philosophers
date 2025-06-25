/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snazzal <snazzal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 11:39:48 by snazzal           #+#    #+#             */
/*   Updated: 2025/06/25 18:48:07 by snazzal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>


typedef struct s_status
{
	int	left_locked;
	int	right_locked;
} t_status;

typedef struct s_forks
{
	int id;
	pthread_mutex_t mutex;
} t_forks;

typedef struct s_data
{
	int				shared;
	int				philos_num;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meals_required;
	long long			start_time;
	int				dead;
	int				is_eating;
	int				ready;
	int				ready_threads;
	int				started_threads;
	pthread_t		monitor;
	// t_philo			*philos;
	t_forks			*forks;
	pthread_mutex_t	print;
	pthread_mutex_t	state;
	pthread_mutex_t	ready_mutex;
}	t_data;

typedef struct s_philo
{
	int				index;
	int				meals_eaten;
	int				last_meal;
	pthread_t		philo;

	pthread_mutex_t	eating;
	t_forks			*left;
	t_forks			*right;

	t_data			*data;
}	t_philo;

#endif
