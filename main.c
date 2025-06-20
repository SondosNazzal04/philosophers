/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snazzal <snazzal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 13:37:47 by snazzal           #+#    #+#             */
/*   Updated: 2025/06/20 18:54:24 by snazzal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_free(t_philo **args)
{
	int	i;

	i = 0;
	if (!args)
		return (0);
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	return (0);
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

void	ft_exit(int exit_signal, char *error_message)
{
	ft_putstr_fd(error_message, 2);
	exit(exit_signal);
}
void	*increment(void *thread)
{
	t_philo	*philo;
	philo = (t_philo *) thread;
	pthread_mutex_lock(&philo->data->fork);
	philo->data->shared++;
	printf("Thread %d incremented shared to %d\n",
		philo->index, philo->data->shared);
	pthread_mutex_unlock(&philo->data->fork);
	return (NULL);
}

int	init(t_philo ***philos, char **argv)
{
	t_data		*data;
	int			i;

	data = malloc(sizeof(t_data));
	if (!data)
		return (0);
	data->shared = 0;
	data->philos_num = ft_atoi(argv[1]);
	*philos = malloc(sizeof(t_philo *) * data->philos_num);
	if (!philos)
	{
		free(data);
		return (0);
	}
	i = 0;
	while (i < ft_atoi(argv[1]))
	{
		(*philos)[i] = malloc(sizeof(t_philo));
		if (!(*philos)[i])
		{
			ft_free(*philos);
			return (0);
		}
		(*philos)[i]->data = data;
		(*philos)[i]->index = i;
		i++;
	}
	return (1);
}

int	main(int argc, char **argv)
{
	int			i;
	t_philo		**philos;

	if (argc != 2)
		ft_exit(1, "Invalid number of arguments");
	philos = NULL;
	if (!init(&philos, argv))
		return (1);
	i = 0;
	pthread_mutex_init(&philos[i]->data->fork, NULL);
	while (i < ft_atoi(argv[1]))
	{
		if (pthread_create(&philos[i]->philo, NULL, increment, philos[i]) != 0)
		{
			perror("Failed to create thread");
			return (1);
		}
		i++;
	}
	i = 0;
	while (i < ft_atoi(argv[1]))
	{
		if (pthread_join(philos[i]->philo, NULL) != 0)
		{
			perror("Failed to join thread");
			return (1);
		}
		i++;
	}
	pthread_mutex_destroy(&philos[0]->data->fork);
	printf("Final value: %d\n", philos[0]->data->shared);
	free(philos[0]->data);
	free(philos);
	return (0);
}
