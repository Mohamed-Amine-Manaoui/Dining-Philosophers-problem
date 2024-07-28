/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmanaoui <mmanaoui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 17:45:10 by mmanaoui          #+#    #+#             */
/*   Updated: 2024/07/28 20:31:37 by mmanaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

// #define nbr_meals 1

struct s_help;
typedef struct s_philo
{
	int				id;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
	size_t			last_time_eat;
	size_t			meals;
	struct s_help	*help;
}					t_philo;

typedef struct s_help
{
	int				nbr_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nbr_meals;
	pthread_t		*t1;
	pthread_mutex_t	*forks;
	pthread_mutex_t	mutex_data;
	pthread_mutex_t	mutex_monitor;
	pthread_mutex_t	mutex_dead;
	struct s_philo	*philo;
	int				deads;
	size_t			start;
	size_t			philo_meals;
	// flag for I have five argv
	size_t			flag_meals;
}					t_help;

#define COLOR_RESET "\033[0m"
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"

/////////////////////// khassni n9ra 3lihoum bzaaaf bzaaaf bzaaaf
size_t	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

int	ft_msleep(size_t milliseconds)
{
	size_t	start;

	start = get_current_time();
	while ((get_current_time() - start) < milliseconds)
		usleep(500);
	return (0);
}

void	take_a_fork(t_philo *philo, size_t current_t)
{
	pthread_mutex_lock(&philo->help->mutex_monitor);
	if (philo->help->deads)
	{
		pthread_mutex_unlock(&philo->help->mutex_monitor);
		return ;
	}
	pthread_mutex_unlock(&philo->help->mutex_monitor);
	pthread_mutex_lock(&philo->help->mutex_data);
	printf(COLOR_YELLOW "%zu %d take a fork !!\n" COLOR_RESET, current_t
		- philo->help->start, philo->id);
	pthread_mutex_unlock(&philo->help->mutex_data);
}

// int message(t_philo *philo, char *str)
// {
// 	pthread_mutex_lock();
// 	if (check_death())
// 	{
// 		pthread_mutex_unlock();
// 		return (1);
// 	}
// 	pthread_mutex_unlock();
// 	pthread_mutex_lock();
// 	printf("%zu %d %s", calculate_time(), philo_id, str);
// 	pthread_mutex_unlock();
// 	return (0);
// }

int	eating(t_philo *philo, size_t current_t)
{
	pthread_mutex_lock(&philo->help->mutex_monitor);
	if (philo->help->deads)
	{
		pthread_mutex_unlock(&philo->help->mutex_monitor);
		return (1);
	}
	pthread_mutex_unlock(&philo->help->mutex_monitor);
	pthread_mutex_lock(&philo->help->mutex_monitor);
	if (philo->help->flag_meals == 1)
	{
		philo->meals--;
		if (philo->meals == 0)
			philo->help->philo_meals--;
	}
	pthread_mutex_unlock(&philo->help->mutex_monitor);
	pthread_mutex_lock(&philo->help->mutex_data);
	printf(COLOR_GREEN "%zu %d EATING\n" COLOR_RESET, current_t
		- philo->help->start, philo->id);
	pthread_mutex_unlock(&philo->help->mutex_data);
	pthread_mutex_lock(&philo->help->mutex_monitor);
	philo->last_time_eat = get_current_time();
	pthread_mutex_unlock(&philo->help->mutex_monitor);
	ft_msleep(philo->help->time_to_eat);
	return (0);
}

int	sleeping(t_philo *philo, size_t current_t)
{
	pthread_mutex_lock(&philo->help->mutex_monitor);
	if (philo->help->deads)
	{
		pthread_mutex_unlock(&philo->help->mutex_monitor);
		return (1);
	}
	pthread_mutex_unlock(&philo->help->mutex_monitor);
	pthread_mutex_lock(&philo->help->mutex_data);
	printf(COLOR_GREEN "%zu %d SLEEPING\n" COLOR_RESET, current_t
		- philo->help->start, philo->id);
	pthread_mutex_unlock(&philo->help->mutex_data);
	ft_msleep(philo->help->time_to_sleep);
	return (0);
}

int	thinking(t_philo *philo, size_t current_t)
{
	pthread_mutex_lock(&philo->help->mutex_monitor);
	if (philo->help->deads)
	{
		pthread_mutex_unlock(&philo->help->mutex_monitor);
		return (1);
	}
	pthread_mutex_unlock(&philo->help->mutex_monitor);
	pthread_mutex_lock(&philo->help->mutex_data);
	printf(COLOR_GREEN "%zu %d THINKING\n" COLOR_RESET, current_t
		- philo->help->start, philo->id);
	pthread_mutex_unlock(&philo->help->mutex_data);
	return (0);
}

void	died(t_philo *philo, size_t current_t)
{
	pthread_mutex_lock(&philo->help->mutex_data);
	printf(COLOR_RED "%zu %d died\n" COLOR_RESET, current_t
		- philo->help->start, philo->id);
	pthread_mutex_unlock(&philo->help->mutex_data);
}

void	dd(t_philo *philo)
{
	pthread_mutex_lock(philo->l_fork);
	take_a_fork(philo, get_current_time());
	pthread_mutex_lock(philo->r_fork);
	take_a_fork(philo, get_current_time());
}
void	ee(t_philo *philo)
{
	pthread_mutex_lock(&philo->help->mutex_monitor);
	if (philo->help->deads)
	{
		pthread_mutex_unlock(&philo->help->mutex_monitor);
		return ;
	}
	pthread_mutex_unlock(&philo->help->mutex_monitor);
}

void	*ss(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(60);
	while (1)
	{
		ee(philo);
		dd(philo);
		if (eating(philo, get_current_time()) == 1)
		{
			pthread_mutex_unlock(philo->r_fork);
			pthread_mutex_unlock(philo->l_fork);
			return (NULL);
		}
		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(philo->l_fork);
		if (sleeping(philo, get_current_time()) == 1)
			return (NULL);
		if (thinking(philo, get_current_time()) == 1)
			return (NULL);
		ft_msleep((philo->help->time_to_die - (get_current_time()
					- philo->last_time_eat)) / 2);
	}
	return (NULL);
}

void	init_mutex(t_help *help)
{
	int	i;

	i = 0;
	help->forks = malloc(help->nbr_philo * sizeof(pthread_mutex_t));
	while (i < help->nbr_philo)
	{
		pthread_mutex_init(&help->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&help->mutex_data, NULL);
	pthread_mutex_init(&help->mutex_monitor, NULL);
	pthread_mutex_init(&help->mutex_dead, NULL);
}

void	init_philo(t_help *help)
{
	int	i;

	help->t1 = malloc(help->nbr_philo * sizeof(pthread_t));
	help->philo = malloc(help->nbr_philo * sizeof(t_philo));
	i = 0;
	help->philo_meals = help->nbr_philo;
	while (i < help->nbr_philo)
	{
		if (help->flag_meals)
			help->philo[i].meals = help->nbr_meals;
		help->philo[i].id = i + 1;
		help->philo[i].help = help;
		help->philo[i].last_time_eat = get_current_time();
		help->philo[i].l_fork = &help->forks[i];
		help->philo[i].r_fork = &help->forks[(i + 1) % help->nbr_philo];
		if (i == help->nbr_philo - 1)
		{
			help->philo[i].r_fork = &help->forks[i];
			help->philo[i].l_fork = &help->forks[(i + 1) % help->nbr_philo];
		}
		i++;
	}
	i = -1;
	while (++i < help->nbr_philo)
		pthread_create(&help->t1[i], NULL, ss, &help->philo[i]);
}

void	join_philo(t_help *help)
{
	int	i;

	i = 0;
	while (i < help->nbr_philo)
	{
		pthread_join(help->t1[i], NULL);
		i++;
	}
}

int	__if_max_meals__(t_help *help)
{
	pthread_mutex_lock(&help->mutex_monitor);
	if (help->philo_meals == 0)
	{
		pthread_mutex_lock(&help->mutex_dead);
		help->deads = 1;
		pthread_mutex_unlock(&help->mutex_dead);
		pthread_mutex_unlock(&help->mutex_monitor);
		return (0);
	}
	pthread_mutex_unlock(&help->mutex_monitor);
	return (1);
}

void	l7day(t_help *help)
{
	int		i;
	size_t	current_time;

	while (1)
	{
		if (help->flag_meals == 1 && __if_max_meals__(help) == 0)
			return ;
		i = 0;
		while (i < help->nbr_philo)
		{
			pthread_mutex_lock(&help->mutex_monitor);
			current_time = get_current_time();
			if (current_time - help->philo[i].last_time_eat > help->time_to_die)
			{
				died(&help->philo[i], get_current_time());
				pthread_mutex_lock(&help->mutex_dead);
				help->deads = 1;
				pthread_mutex_unlock(&help->mutex_dead);
				pthread_mutex_unlock(&help->mutex_monitor);
				return ;
			}
			pthread_mutex_unlock(&help->mutex_monitor);
			i++;
		}
	}
}
//////////////////////______PARSING______/////////////////////////

int	ft_atoi(const char *str)
{
	int		i;
	int		signe;
	long	res;

	i = 0;
	signe = 1;
	res = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+')
		i++;
	else if (str[i] == '-')
	{
		signe = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i] - '0');
		if (res * signe > INT_MAX || res * signe < INT_MIN)
			printf("Error overflow !!\n");
		i++;
	}
	return (res * signe);
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

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
	return (1); // Success
}

int	valid_args(char **av, t_help *help)
{
	int	i;

	i = 1;
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
	// exit(0);
	return (0);
}

int	main(int ac, char **av)
{
	t_help	*help;

	help = malloc(sizeof(t_help));
	if (ac == 5 || ac == 6)
	{
		if (valid_args(av, help) == 1)
			return (printf("INVALID ARGUMENT !!!!\n"), 0);
		help->start = get_current_time();
		help->deads = 0;
		help->philo_meals = help->nbr_philo;
		// for now before parsing
		help->flag_meals = 1;
		init_mutex(help);
		init_philo(help);
		l7day(help);
		join_philo(help);
	}
	else
		printf("number for argument invalid !!!\n");
	return (0);
}

// int	main(void)
// {
// 	t_help	*help;

// 	help = malloc(sizeof(t_help));
// 	help->start = get_current_time();
// 	help->deads = 0;
// 	help->philo_meals = nbr_philo;
// 	// for now before parsing
// 	help->flag_meals = 1;
// 	init_mutex(help);
// 	init_philo(help);
// 	l7day(help);
// 	join_philo(help);
// 	return (0);
// }

// philo ==> thread
// forks ==> mutex