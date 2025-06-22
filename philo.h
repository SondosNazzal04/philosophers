/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snazzal <snazzal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 11:39:48 by snazzal           #+#    #+#             */
/*   Updated: 2025/06/22 12:06:12 by snazzal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_data
{
	int				shared;
	int				philos_num;
	pthread_mutex_t	*forks;
	pthread_mutex_t	mutex;
}	t_data;

typedef struct s_philo
{
	int				index;
	pthread_t		philo;
	pthread_mutex_t	*left;
	pthread_mutex_t	*right;

	t_data			*data;
}	t_philo;

#endif
