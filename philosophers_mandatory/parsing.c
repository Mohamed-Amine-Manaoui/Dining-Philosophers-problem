/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmanaoui <mmanaoui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 21:45:26 by mmanaoui          #+#    #+#             */
/*   Updated: 2024/08/08 23:53:09 by mmanaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
		while (str[i] == 32)
			i++;
		if (((str[i] == '-' || str[i] == '+') && !(str[i + 1] >= 48 && str[i
						+ 1] <= 57)) || check_alpha(str))
		{
			return (-1);
		}
		i++;
	}
	return (1);
}

void	handle_whitespaces(int ac, char **av)
{
	int	i;
	int	j;
	int	k;

	i = 1;
	k = 0;
	while (av[i])
	{
		j = 0;
		while (av[i][j] == 32 || av[i][j] == 9)
			j++;
		if (!av[i][j])
			break ;
		k = j + 1;
		while (av[i][k] && av[i][k] != 32 && av[i][k] != 9)
			k++;
		av[i] = ft_substr(av[i], j, k - j);
		if (ac - 1 == i)
			break ;
		i++;
	}
}

int	valid_args(char **av, int ac, t_help *help)
{
	int	i;

	i = 1;
	handle_whitespaces(ac, av);
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

int	count_spaces(char *str, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == c)
			count++;
		i++;
	}
	return (count);
}
