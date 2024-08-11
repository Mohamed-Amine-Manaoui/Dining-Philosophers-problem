/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmanaoui <mmanaoui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 21:30:23 by mmanaoui          #+#    #+#             */
/*   Updated: 2024/08/11 07:14:14 by mmanaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	check_alpha(char *av)
{
	int	i;

	i = 0;
	if (av[i] == '-' || av[i] == '+')
		i++;
	if (!av[i])
		return (1);
	while (av[i])
	{
		if (ft_isdigit(av[i]))
			i++;
		else
			return (1);
	}
	return (0);
}

int	handle_error(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (((str[i] == '-' || str[i] == '+') && !(str[i + 1] >= 48 && str[i
						+ 1] <= 57)) || check_alpha(str))
		{
			return (-1);
		}
		i++;
	}
	return (1);
}

void	handle_whitespaces(char **av)
{
	int	i;
	int	j;

	i = 1;
	while (av[i])
	{
		j = 0;
		while (av[i][j] == 32 || av[i][j] == 9)
			j++;
		av[i] = av[i] + j;
		j = ft_strlen(av[i]) - 1;
		while (av[i][j] == 32 || av[i][j] == 9)
			j--;
		av[i][j + 1] = '\0';
		i++;
	}
}

int	valid_args(char **av, t_help *help)
{
	int	i;

	i = 1;
	handle_whitespaces(av);
	while (av[i])
	{
		if (handle_error(av[i]) == -1)
			return (1);
		i++;
	}
	help->nbr_philo = ft_atoi(av[1]);
	help->time_to_die = ft_atoi(av[2]);
	help->time_to_eat = ft_atoi(av[3]);
	help->time_to_sleep = ft_atoi(av[4]);
	if (av[5])
		help->nbr_meals = ft_atoi(av[5]);
	return (0);
}
