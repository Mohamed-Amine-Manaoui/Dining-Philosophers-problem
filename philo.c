#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#define NBR_PHILO 5
#define TIME_TO_DIE 800
#define TIME_TO_EAT 200
#define TIME_TO_SLEEP 200
#define NBR_OF_x7al_khasso_yakl 10

struct s_help;
typedef struct s_philo
{
	int				id;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
	size_t			last_time_eat;
	struct s_help	*help;
}					t_philo;

typedef struct s_help
{
	pthread_t		*t1;
	pthread_mutex_t	*forks;
	pthread_mutex_t	mutex_data;
	pthread_mutex_t	mutex_monitor;
	pthread_mutex_t	mutex_dead;
	struct s_philo	*philo;
	int				deads;
	size_t			start;
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

int	ft_usleep(size_t milliseconds)
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
// 		return 1;
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
	pthread_mutex_lock(&philo->help->mutex_data);
	printf(COLOR_GREEN "%zu %d EATING\n" COLOR_RESET, current_t
		- philo->help->start, philo->id);
	pthread_mutex_unlock(&philo->help->mutex_data);
	pthread_mutex_lock(&philo->help->mutex_monitor);
	philo->last_time_eat = get_current_time();
	pthread_mutex_unlock(&philo->help->mutex_monitor);
	ft_usleep(TIME_TO_EAT);
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

void	*ss(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	// if (philo->id % 2 == 0)
	// 	ft_usleep(60);
	while (1)
	{
		pthread_mutex_lock(&philo->help->mutex_monitor);
		if (philo->help->deads)
		{
			pthread_mutex_unlock(&philo->help->mutex_monitor);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->help->mutex_monitor);
		pthread_mutex_lock(philo->l_fork);
		take_a_fork(philo, get_current_time());
		pthread_mutex_lock(philo->r_fork);
		take_a_fork(philo, get_current_time());
		if (eating(philo, get_current_time()) == 1)
		{
			pthread_mutex_unlock(philo->r_fork);
			pthread_mutex_unlock(philo->l_fork);
			return (NULL);
		}
		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(philo->l_fork);
		if (sleeping(philo, get_current_time()) == 1)
		{
			return (NULL);
		}
	ft_usleep(TIME_TO_SLEEP);
		if (thinking(philo, get_current_time()) == 1)
		{
			return (NULL);
		}
	}
	return (NULL);
}

void	init_mutex(t_help *help)
{
	int	i;

	i = 0;
	help->forks = malloc(NBR_PHILO * sizeof(pthread_mutex_t));
	while (i < NBR_PHILO)
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

	i = 0;
	help->t1 = malloc(NBR_PHILO * sizeof(pthread_t));
	help->philo = malloc(NBR_PHILO * sizeof(t_philo));
	while (i < NBR_PHILO)
	{
		help->philo[i].id = i + 1;
		help->philo[i].help = help;
		help->philo[i].last_time_eat = get_current_time();
		help->philo[i].l_fork = &help->forks[i];
		help->philo[i].r_fork = &help->forks[(i + 1) % NBR_PHILO];
		if (i == NBR_PHILO - 1)
		{
			help->philo[i].r_fork = &help->forks[i];
			help->philo[i].l_fork = &help->forks[(i + 1) % NBR_PHILO];	
		}
		pthread_create(&help->t1[i], NULL, ss, &help->philo[i]);
		i++;
	}
}

void	join_philo(t_help *help)
{
	int	i;

	i = 0;
	while (i < NBR_PHILO)
	{
		pthread_join(help->t1[i], NULL);
		i++;
	}
}

void	l7day(t_help *help)
{
	int		i;
	size_t	current_time;

	while (1)
	{
		i = 0;
		while (i < NBR_PHILO)
		{
			pthread_mutex_lock(&help->mutex_monitor);
			current_time = get_current_time();
			if (current_time - help->philo[i].last_time_eat > TIME_TO_DIE)
			{
				printf("SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS\n");
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
		// ft_usleep(1000);
	}
}

int	main(void)
{
	t_help	*help;

	help = malloc(sizeof(t_help));
	help->start = get_current_time();
	help->deads = 0;
	init_mutex(help);
	init_philo(help);
	l7day(help);
	join_philo(help);
	return (0);
}

// philo ==> thread
// forks ==> mutex