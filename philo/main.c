/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snazzal <snazzal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 12:06:19 by snazzal           #+#    #+#             */
/*   Updated: 2025/06/26 17:52:52 by snazzal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_data	data;

	memset(&data, 0, sizeof(t_data));
	if (parse_args(&data, argc, argv) == 0)
		return (EXIT_FAILURE);
	if (!init_forks(&data) || !philo_init(&data) || !init_threads(&data))
	{
		clean_up(&data);
		return (EXIT_FAILURE);
	}
	clean_up(&data);
	return (EXIT_SUCCESS);
}
