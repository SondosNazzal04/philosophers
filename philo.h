/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snazzal <snazzal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 11:39:48 by snazzal           #+#    #+#             */
/*   Updated: 2025/06/17 18:16:24 by snazzal          ###   ########.fr       */
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
	int shared;
	pthread_mutex_t fork;
} t_data;

typedef struct s_philo
{
	int	index;
	pthread_t philo;
	t_data *data;
} t_philo;

#endif
