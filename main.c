/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snazzal <snazzal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 13:37:47 by snazzal           #+#    #+#             */
/*   Updated: 2025/06/22 12:08:13 by snazzal          ###   ########.fr       */
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

void	cleanup(t_philo **philos, t_data *data)
{
	if (data->forks)
		free(data->forks);
	if (data)
		free(data);
	if (philos)
		ft_free(philos);
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

void	ft_putstr_fd(char *s, int fd)
{
	while (*s)
	{
		write(fd, &*s, 1);
		s++;
	}
}

void	*increment(void *thread)
{
	t_philo	*philo;

	philo = (t_philo *) thread;
	usleep(50);
	if (philo->index % 2 == 0)
	{
		pthread_mutex_lock(philo->right);
		pthread_mutex_lock(philo->left);
		pthread_mutex_lock(&philo->data->mutex);
		philo->data->shared++;
		printf("Thread %d incremented shared to %d\n",
			philo->index, philo->data->shared);
		usleep(1000);
		pthread_mutex_unlock(&philo->data->mutex);
		pthread_mutex_unlock(philo->left);
		pthread_mutex_unlock(philo->right);
		usleep(1000);
	}
	else
	{
		pthread_mutex_lock(philo->left);
		pthread_mutex_lock(philo->right);
		pthread_mutex_lock(&philo->data->mutex);
		philo->data->shared++;
		printf("Thread %d incremented shared to %d\n",
			philo->index, philo->data->shared);
		usleep(1000);
		pthread_mutex_unlock(&philo->data->mutex);
		pthread_mutex_unlock(philo->right);
		pthread_mutex_unlock(philo->left);
		usleep(1000);
	}
	return (NULL);
}

int	init_philos(t_philo **philos, t_data *data)
{
	int	i;

	i = 0;
	data->forks = malloc(sizeof(pthread_mutex_t)
				* (data->philos_num));
	if (!data->forks)
	{
		ft_free(philos);
		return (0);
	}
	while (i < data->philos_num)
	{
		philos[i] = malloc(sizeof(t_philo));
		if (!philos[i])
		{
			ft_free(philos);
			return (0);
		}
		philos[i]->data = data;
		philos[i]->index = i;
		pthread_mutex_init(&data->forks[i], NULL);
		philos[i]->left = &data->forks[i];
		if (i == data->philos_num - 1)
			philos[i]->right = &data->forks[0];
		else
			philos[i]->right = &data->forks[i + 1];
		i++;
	}
	pthread_mutex_init(&data->mutex, NULL);
	philos[data->philos_num] = NULL;
	return (1);
}

t_philo	**init(char **argv, t_data **data)
{
	t_philo		**philos;

	*data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	(*data)->shared = 0;
	(*data)->philos_num = ft_atoi(argv[1]);
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

int	create_philos(t_philo **philos)
{
	int	i;

	i = 0;
	if (!philos || !philos[0])
		return (0);
	while (i < philos[0]->data->philos_num)
	{
		pthread_mutex_init(&philos[0]->data->forks[i], NULL);
		if (pthread_create(&philos[i]->philo, NULL,
				increment, philos[i]) != 0)
		{
			perror("Failed to create thread");
			return (0);
		}
		i++;
	}
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

int	destroy_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philos_num)
	{
		// printf("thread %i right %p\n", i, philos[i]->right);
		// printf("thread %i left %p\n", i, philos[i]->left);
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	return (1);
}

int	main(int argc, char **argv)
{
	t_philo	**philos;
	t_data	*data;
	// int		i;

	if (argc != 2)
	{
		ft_putstr_fd("Invalid number of arguments.\n", 2);
		return (1);
	}
	philos = init(argv, &data);
	if (!philos)
		return (1);
	if (!create_philos(philos))
	{
		cleanup(philos, data);
		return (1);
	}
	if (!join_philos(philos))
	{
		cleanup(philos, data);
		return (1);
	}
	if (!destroy_forks(data))
	{
		cleanup(philos, data);
		return (1);
	}
	// i = 0;
	// while (i < data->philos_num)
	// {
	// 	// printf("thread %i right %p\n", i, philos[i]->right);
	// 	// printf("thread %i left %p\n", i, philos[i]->left);
	// 	pthread_mutex_destroy(&data->forks[i]);
	// 	i++;
	// }
	printf("Final value: %d\n", philos[0]->data->shared);
	cleanup(philos, data);
	return (0);
}
