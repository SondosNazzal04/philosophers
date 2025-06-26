/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snazzal <snazzal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 11:39:48 by snazzal           #+#    #+#             */
/*   Updated: 2025/06/26 17:54:20 by snazzal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <string.h>

typedef struct s_forks
{
	int				id;
	pthread_mutex_t	mutex;
}	t_forks;

typedef struct s_philo	t_philo;

typedef struct s_data
{
	int				n_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meals_required;
	long long		start_time;
	int				is_stopped;
	int				ready;
	pthread_t		monitor;
	t_philo			*philos;
	t_forks			*forks;
	pthread_mutex_t	print;
	pthread_mutex_t	state;
}	t_data;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long long		last_meal;
	int				is_eating;
	pthread_t		philo_id;
	pthread_mutex_t	eating;
	t_forks			*left;
	t_forks			*right;
	t_data			*data;
}	t_philo;

/***************utils funcitons**********/
int			ft_atoi(const char *nptr);
void		print_state(char *string, t_philo *philo);
void		clean_up(t_data *data);
int			join_threads(t_data *data);
void		update_meal_state(t_philo *philo, int flag);
/****************time functions**********/
int			inter_milisleep(t_data *data, long long time);
long long	get_time(void);
void		wait_threads(t_philo *philo);
/***************forks functions**********/
void		setup_forks(t_philo *philo, t_forks **first, t_forks **second);
int			take_forks(t_philo *philo, t_forks *first, t_forks *second);
int			init_forks(t_data *data);
/************checking functions**********/
int			done_eating(t_philo *philo);
int			check_dead(t_data *data);
int			check_all_death(t_data *data);
int			check_death(t_philo *philo);
int			has_eaten_enough(t_data *data);
/*************routine functions**********/
int			eating(t_philo *philo);
int			thinking(t_philo *philo);
int			sleeping(t_philo *philo);
void		*routine_philo(void *args);
/*************init functions**************/
int			philo_init(t_data *data);
int			parse_args(t_data *data, int argc, char **argv);
int			init_threads(t_data *data);
/*monitor functions*/
void		wait_threads_m(t_data *data);
void		*monitor(void *args);

#endif
