/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snazzal <snazzal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 13:37:47 by snazzal           #+#    #+#             */
/*   Updated: 2025/06/17 17:20:30 by snazzal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
int shared;
pthread_mutex_t	lock;

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
	pthread_mutex_lock(&lock);
	shared++;
	printf("Thread %ld incremented shared to %d\n", (long)thread, shared);
	pthread_mutex_unlock(&lock);
	return (NULL);
}

int	main(int argc, char **argv)
{
	int			i;
	pthread_t	*philos;
	// (void) argv;
	(void) argc;
	// if (argc != 6 && argc != 5)
	// 	ft_exit(1, "Invalid number of arguments");
	philos = malloc(sizeof(pthread_t) * ft_atoi(argv[1]));
	i = 0;
	pthread_mutex_init(&lock, NULL);

	while (i < ft_atoi(argv[1]))
	{
		if (pthread_create(&philos[i], NULL, increment, (void *)(__intptr_t)i) != 0)
		{
			perror("Failed to create thread");
			return (1);
		}
		i++;
	}
	i = 0;
	while (i < ft_atoi(argv[1]))
	{
		if (pthread_join(philos[i], NULL) != 0)
		{
			perror("Failed to join thread");
			return (1);
		}
		i++;
	}
	pthread_mutex_destroy(&lock);
	printf("Final value: %d\n", shared);
	free(philos);
	return (0);
}
