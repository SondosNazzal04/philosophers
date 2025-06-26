/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snazzal <snazzal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 13:37:47 by snazzal           #+#    #+#             */
/*   Updated: 2025/06/26 15:58:19 by snazzal          ###   ########.fr       */
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
		if (pthread_mutex_destroy(&data->forks[i].mutex) != 0)
			ft_putstr_fd("Failed to destroy fork\n", 2);
		if (pthread_mutex_destroy(&philo[i]->eating) != 0)
			ft_putstr_fd("Failed to destroy eating lock\n", 2);
		i++;
	}
	if (pthread_mutex_destroy(&data->state) != 0)
		ft_putstr_fd("Failed to destroy fork\n", 2);
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

long long	get_time(void)
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
		return (1);
	}
	if (
		// philo->data->meals_required > 0
		// &&
		 philo->meals_eaten >= philo->data->meals_required)
		return (1);
	return (0);
}

void	print_state(char *string, t_philo *philo)
{
	long long	timestamp;

	pthread_mutex_lock(&philo->data->state);
	timestamp = get_time() - philo->data->start_time;
	pthread_mutex_unlock(&philo->data->state);
	pthread_mutex_lock(&philo->data->print);
	printf("%lld %d %s\n",
		timestamp, philo->index, string);
	pthread_mutex_unlock(&philo->data->print);
}

void	think(t_philo *philo)
{
	if (should_stop(philo))
		return ;
	print_state("is thinking 🤔", philo);
	usleep(500);
}

int	take_forks(t_philo *philo)
{
	t_forks	*first;
	t_forks	*second;
	//okay
	if (should_stop(philo))
			return (0);
	if (philo->index % 2 == 0)
	{
		first = philo->right;
		second = philo->left;
	}
	else
	{
		first = philo->left;
		second = philo->right;
	}
	pthread_mutex_lock(&first->mutex);
	if (should_stop(philo))
	{
		pthread_mutex_unlock(&first->mutex);
		return (0);
	}
	print_state("has taken a fork 🍴", philo);
	pthread_mutex_lock(&second->mutex);
	if (should_stop(philo))
	{
		pthread_mutex_unlock(&first->mutex);
		pthread_mutex_unlock(&second->mutex);
		return (0);
	}
	print_state("has taken a fork 🍴", philo);
	philo->data->is_eating = 1;
	return (1);
}

void	sync_start(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->state);
	while (!philo->data->ready_threads)
	{
		pthread_mutex_unlock(&philo->data->state);
		usleep(100);
		pthread_mutex_lock(&philo->data->state);
	}
	pthread_mutex_unlock(&philo->data->state);
}

void	put_forks(t_philo *philo)
{
	pthread_mutex_unlock(&philo->left->mutex);
	pthread_mutex_unlock(&philo->right->mutex);
}

void	sleeping(t_philo *philo)
{
	if (should_stop(philo))
		return ;
	print_state("is sleeping 💤", philo);
	milisleep(philo->data->time_to_sleep);
}

int	eating(t_philo *philo)
{
	if (should_stop(philo))
		return (0);
	pthread_mutex_lock(&philo->eating);
	philo->last_meal = get_time();
	milisleep(philo->data->time_to_eat);
	// philo->eating = 0;
	// philo->data->is_eating = 0;
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->eating);
	print_state("is eating 🍝", philo);
	return (1);
}

// void wait_threads(t_philo *philo)
// {
// 	pthread_mutex_lock(&philo->data->state);
// 	while(philo->data->ready_threads)
// 	{
// 		pthread_mutex_unlock(&philo->data->state);
// 		usleep(100);
// 		pthread_mutex_lock(&philo->data->state);
// 	}
// 	pthread_mutex_unlock(&philo->data->state);
// }

void	init_philo_state(t_philo *philo)
{
	// wait_threads(philo);
	sync_start(philo);
	if (philo->index % 2 == 0)
		usleep(1000);
	pthread_mutex_lock(&philo->eating);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->eating);
}

void	*routine(void *thread)
{
	t_philo		*philo;

	philo = (t_philo *) thread;
	init_philo_state(philo);
	while (1)
	{
		if(should_stop(philo))
			break;
		if (!take_forks(philo))
			break ;
		if (!eating(philo))
			break ;
		put_forks(philo);
		think(philo);
		sleeping(philo);
	}
	return (NULL);
}

void	init_forks(t_forks *forks, int philos_num)
{
	int	i;

	i = 0;
	while (i < philos_num)
	{
		forks[i].id = i;
		forks[i].mutex = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
		i++;
	}
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
		// philos[i]->last_meal = get_time();
		philos[i]->meals_eaten = 0;
		// data->forks->mutex = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
		// data->forks->id = i;
		// pthread_mutex_init(&data->forks->mutex, NULL);
		philos[i]->left = &data->forks[i];
		if (i == data->philos_num - 1)
			philos[i]->right = &data->forks[0];
		else
			philos[i]->right = &data->forks[i + 1];
		// pthread_mutex_init(&philos[i]->eating, NULL);
		philos[i]->eating = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
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
	(*data)->forks = malloc(sizeof(t_forks)
			* ((*data)->philos_num));
	init_forks((*data)->forks, (*data)->philos_num);
	if (!(*data)->forks)
		return (0);
	// (*data)->shared = 0;
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
	(*data)->ready_mutex = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
	// pthread_mutex_init(&(*data)->ready_mutex, NULL);
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

void	*monitor(void *arg)
{
	t_philo	**philos;
	int		i;

	philos = (t_philo **) arg;
	while (1)
	{
		i = 0;
		while (philos[i])
		{
			pthread_mutex_lock(&philos[i]->eating);
			long time_since_last_meal = get_time() - philos[i]->last_meal;
			if (time_since_last_meal > philos[i]->data->time_to_die)
			{
				pthread_mutex_lock(&philos[i]->data->state);
				philos[i]->data->dead = 1;
				pthread_mutex_unlock(&philos[i]->eating);
				pthread_mutex_unlock(&philos[i]->data->state);

				pthread_mutex_lock(&philos[i]->data->print);
				printf("%lld %d died 💀\n",
					get_time() - philos[i]->data->start_time,
					philos[i]->index);
				pthread_mutex_unlock(&philos[i]->data->print);
				return (NULL);
			}
			pthread_mutex_unlock(&philos[i]->eating);
			i++;
		}
		usleep(10);
	}
	return (NULL);
}


int	create_philos(t_philo **philos)
{
	int		i;
	// long	start;

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
	pthread_mutex_lock(&(*philos)->data->ready_mutex);
	(*philos)->data->ready_threads = 1;
	pthread_mutex_unlock(&(*philos)->data->ready_mutex);

	// while (1)
	// {
	// 	pthread_mutex_lock(&(*philos)->data->ready_mutex);
	// 	if ((*philos)->data->ready_threads == (*philos)->data->philos_num)
	// 	{
	// 		pthread_mutex_unlock(&(*philos)->data->ready_mutex);
	// 		break;
	// 	}
	// 	pthread_mutex_unlock(&(*philos)->data->ready_mutex);
	// 	usleep(100);
	// }
	(*philos)->data->start_time = get_time(); // start_time is here wooo
	// i = 0;
	// while (i < philos[0]->data->philos_num)
	// 	philos[i++]->last_meal = start;
	// pthread_mutex_lock(&philos[0]->data->state);
	// philos[0]->data->ready = 1;
	// pthread_mutex_unlock(&philos[0]->data->state);
	// usleep(500);
	pthread_create(&(*philos)->data->monitor, NULL, monitor, philos);
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
	if (pthread_join((*philos)->data->monitor, NULL) != 0)
	{
		ft_putstr_fd("Failed to join monitor thread.\n", 2);
		return (0);
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
