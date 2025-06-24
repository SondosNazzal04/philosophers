/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snazzal <snazzal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 13:37:47 by snazzal           #+#    #+#             */
/*   Updated: 2025/06/24 16:10:28 by snazzal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	fail_flag = 1;

int	ft_free(t_philo **args)
{
	int	i;

	i = 0;
	if (!args || !args[0])
		return (0);
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
	return (0);
}

void	ft_putstr_fd(char *s, int fd)
{
	while (*s)
	{
		write(fd, &*s, 1);
		s++;
	}
}

int	destroy_locks(t_philo **philo, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philos_num)
	{
		if (pthread_mutex_destroy(&data->forks[i]) != 0)
			ft_putstr_fd("Failed to destroy fork", 2);
		if (pthread_mutex_destroy(&philo[i]->eating) != 0)
			ft_putstr_fd("Failed to destroy eating lock", 2);
		i++;
	}
	if (pthread_mutex_destroy(&data->state) != 0)
		ft_putstr_fd("Failed to destroy fork", 2);
	// if (pthread_mutex_destroy(&data->state) != 0)
	// 	ft_putstr_fd("Failed to destroy death lock", 2);
	return (1);
}

int	cleanup(t_philo **philos, t_data *data, int i)
{
	// if (data->forks)
	// 	destroy_locks(philos, data);
	if (data->forks)
		free(data->forks);
	if (data)
		free(data);
	if (philos)
		ft_free(philos);
	return (i);
}

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

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void milisleep(long time)
{
	long current_time;

	current_time = get_time();
	while ((get_time() - current_time) < time)
		usleep(100);
}

int	should_stop(t_philo *philo)
{
	int	stop;

	pthread_mutex_lock(&philo->data->state);
	stop = philo->data->dead;
	pthread_mutex_unlock(&philo->data->state);
	if (stop)
	{
		printf("%ld %d died 💀\n",
			(get_time() - philo->data->start_time), philo->index);
		return (1);
	}
	if (philo->data->meals_required > 0
		&& philo->meals_eaten >= philo->data->meals_required)
		return (1);
	return (0);
}

void	think(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->print);
	printf("%ld %d is thinking 🤔\n",
		(get_time() - philo->data->start_time), philo->index);
	pthread_mutex_unlock(&philo->data->print);
	usleep(500);
}

t_status	take_forks(t_philo *philo)
{
	t_status	status;

	status.right_locked = 0;
	status.left_locked = 0;
	if (philo->index % 2 == 0)
	{
		if (pthread_mutex_lock(philo->right) == 0)
			status.right_locked = 1;
		pthread_mutex_lock(&philo->data->print);
		printf("%ld %d has taken a fork 🍴\n",
			(get_time() - philo->data->start_time), philo->index);
		pthread_mutex_unlock(&philo->data->print);
		if (pthread_mutex_lock(philo->left) == 0)
			status.left_locked = 1;
		pthread_mutex_lock(&philo->data->print);
		printf("%ld %d has taken a fork 🍴\n",
			(get_time() - philo->data->start_time), philo->index);
		pthread_mutex_unlock(&philo->data->print);
		philo->data->is_eating = 1;
	}
	else
	{
		if (pthread_mutex_lock(philo->left) == 0)
			status.left_locked = 1;
		pthread_mutex_lock(&philo->data->print);
		printf("%ld %d has taken a fork 🍴\n",
			(get_time() - philo->data->start_time), philo->index);
		pthread_mutex_unlock(&philo->data->print);
		if (pthread_mutex_lock(philo->right) == 0)
			status.right_locked = 1;
		pthread_mutex_lock(&philo->data->print);
		printf("%ld %d has taken a fork 🍴\n",
			(get_time() - philo->data->start_time), philo->index);
		pthread_mutex_unlock(&philo->data->print);
		philo->data->is_eating = 1;
	}
	return (status);
}

void	wait_for_all_threads(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->state);
	while (!philo->data->ready)
	{
		pthread_mutex_unlock(&philo->data->state);
		usleep(100);
		pthread_mutex_lock(&philo->data->state);
	}
	pthread_mutex_unlock(&philo->data->state);
}

void	put_forks(t_philo *philo, t_status status)
{
	if (status.left_locked)
		pthread_mutex_unlock(philo->left);
	if (status.right_locked)
		pthread_mutex_unlock(philo->right);
}

void	sleeping(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->print);
	printf("%ld %d is sleeping 💤\n",
		(get_time() - philo->data->start_time), philo->index);
	pthread_mutex_unlock(&philo->data->print);
	milisleep(philo->data->time_to_sleep);
}

void	eating(t_philo *philo)
{
	pthread_mutex_lock(&philo->eating);
	philo->last_meal = get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->eating);
	pthread_mutex_lock(&philo->data->print);
	printf("%ld %d is eating 🍝\n",
		(get_time() - philo->data->start_time), philo->index);
	pthread_mutex_unlock(&philo->data->print);
	milisleep(philo->data->time_to_sleep);
}

void	*routine(void *thread)
{
	t_philo		*philo;
	t_status	status;

	philo = (t_philo *) thread;
	// wait_for_all_threads(philo);
	while (!should_stop(philo))
	{
		think(philo);
		status = take_forks(philo);
		eating(philo);
		put_forks(philo, status);
		sleeping(philo);
	}
	return (NULL);
}

int	init_philos(t_philo **philos, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philos_num)
	{
		philos[i] = malloc(sizeof(t_philo));
		if (!philos[i])
		{
			ft_free(philos);
			return (0);
		}
		philos[i]->data = data;
		philos[i]->index = i + 1;
		philos[i]->last_meal = get_time();
		philos[i]->meals_eaten = 0;
		pthread_mutex_init(&data->forks[i], NULL);
		philos[i]->left = &data->forks[i];
		if (i == data->philos_num - 1)
			philos[i]->right = &data->forks[0];
		else
			philos[i]->right = &data->forks[i + 1];
		pthread_mutex_init(&philos[i]->eating, NULL);
		i++;
	}
	(*philos)->data->monitor = 0;
	philos[data->philos_num] = NULL;
	return (1);
}

int	init_data(char **argv, t_data **data)
{
	*data = malloc(sizeof(t_data));
	if (!*data)
		return (0);
	(*data)->philos_num = ft_atoi(argv[1]);
	(*data)->start_time = get_time();
	(*data)->forks = malloc(sizeof(pthread_mutex_t)
			* ((*data)->philos_num));
	if (!(*data)->forks)
		return (0);
	(*data)->shared = 0;
	(*data)->dead = 0;
	(*data)->time_to_die = ft_atoi(argv[2]);
	(*data)->time_to_eat = ft_atoi(argv[3]);
	(*data)->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		(*data)->meals_required = ft_atoi(argv[5]);
	else
		(*data)->meals_required = 0;
	(*data)->print = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
	(*data)->state = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
	return (1);
}

t_philo	**init(char **argv, t_data **data)
{
	t_philo		**philos;

	if (!init_data(argv, data))
		return (NULL);
	if ((*data)->philos_num <= 0 || (*data)->philos_num > 200)
	{
		free(*data);
		ft_putstr_fd("Invalid number of philosophers.\n", 2);
		return (NULL);
	}
	philos = malloc(sizeof(t_philo *) * ((*data)->philos_num + 1));
	if (!philos)
	{
		free(*data);
		free((*data)->forks);
		return (NULL);
	}
	if (!init_philos(philos, *data))
		return (NULL);
	return (philos);
}

void	*monitor(void *thread)
{
	t_philo *philo;

	philo = (t_philo *) thread;
	while (1)
	{
		if (philo->last_meal - get_time() >= philo->data->time_to_die)
			philo->data->dead = 1;
	}
}

int	create_philos(t_philo **philos)
{
	int	i;

	i = 0;
	if (!philos || !philos[0])
		return (0);
	while (i < philos[0]->data->philos_num)
	{
		if (pthread_create(&philos[i]->philo, NULL,
				routine, philos[i]) != 0)
		{
			perror("Failed to create thread");
			return (0);
		}
		i++;
	}
	pthread_create(&(*philos)->data->monitor, NULL, monitor, *philos);
	return (1);
}

int	join_philos(t_philo **philos)
{
	int	i;

	i = 0;
	while (i < (*philos)->data->philos_num)
	{
		if (pthread_join(philos[i]->philo, NULL) != 0)
		{
			ft_putstr_fd("Failed to join thread.\n", 2);
			return (0);
		}
		i++;
	}
	return (1);
}

int	main(int argc, char **argv)
{
	t_philo	**philos;
	t_data	*data;

	if (argc != 5 && argc != 6)
	{
		ft_putstr_fd("Invalid number of arguments.\n", 2);
		return (1);
	}
	philos = init(argv, &data);
	if (!philos)
	{
		cleanup(philos, data, 1);
		return (1);
	}
	if (!create_philos(philos))
	{
		cleanup(philos, data, 1);
		return (1);
	}
	if (!join_philos(philos))
	{
		cleanup(philos, data, 1);
		return (1);
	}
	if (!destroy_locks(philos, data))
	{
		cleanup(philos, data, 1);
		return (1);
	}
	cleanup(philos, data, 0);
	return (0);
}
