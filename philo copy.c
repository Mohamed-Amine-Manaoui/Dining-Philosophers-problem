#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#define NBR_PHILO 5
# define TIME_TO_EAT 200
# define TIME_TO_SLEEP 200
# define TIME_TO_DIE 60
# define NBR_OF_x7al_khasso_yakl 10

struct s_help;
typedef struct s_philo
{
	int				id;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
	size_t last_time_eat;
	struct s_help *help;	
}					t_philo;

typedef struct s_help
{
	pthread_t		*t1;
	pthread_mutex_t	*forks;
	pthread_mutex_t	mutex_data;
	struct s_philo	*philo;

	int deads;
	size_t start;
}					t_help;

#define COLOR_RESET "\033[0m"
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"

/////////////////////// khassni n9ra 3lihoum bzaaaf
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

void	*ss(void *arg)
{
	t_philo	*philo;
	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
	ft_usleep(60);
	while (1)
	{
		pthread_mutex_lock(&philo->help->mutex_data);
		if (philo->help->deads)
		{
			printf("DIE !!!\n");
			pthread_mutex_unlock(&philo->help->mutex_data);
			return NULL;
		}
		pthread_mutex_unlock(&philo->help->mutex_data);
		
		pthread_mutex_lock(philo->l_fork);
		if (philo->help->deads)
		{
			pthread_mutex_unlock(philo->l_fork);
			printf("DIE !!!\n");
			return NULL;
		}
        printf(COLOR_YELLOW "%zu %d take a fork !!\n" COLOR_RESET, get_current_time() - philo->help->start , philo->id);
        pthread_mutex_lock(philo->r_fork);
		if (philo->help->deads)
		{
			pthread_mutex_unlock(philo->r_fork);
			printf("DIE !!!\n");
			return NULL;
		}
        printf(COLOR_YELLOW "%zu %d take a fork !!\n" COLOR_RESET, get_current_time() - philo->help->start , philo->id);
		printf(COLOR_GREEN "%zu %d EATING\n" COLOR_RESET, get_current_time() - philo->help->start , philo->id);
		pthread_mutex_lock(&philo->help->mutex_data);
		philo->last_time_eat = get_current_time();
		pthread_mutex_unlock(&philo->help->mutex_data);
		ft_usleep(TIME_TO_EAT);
		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(philo->l_fork);
        printf(COLOR_GREEN "%zu %d SLEEPING\n" COLOR_RESET, get_current_time() - philo->help->start , philo->id);
		ft_usleep(TIME_TO_SLEEP);
        printf(COLOR_GREEN "%zu %d THINKING\n" COLOR_RESET, get_current_time() - philo->help->start , philo->id);
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
}

void	init_philo(t_help *help)
{
	int	i;

	i = 0;
	help->t1 = malloc(NBR_PHILO * sizeof(pthread_t));
	// init struct philo
	help->philo = malloc(NBR_PHILO * sizeof(t_philo));
	while (i < NBR_PHILO)
	{
		help->philo[i].id = i + 1;
		help->philo[i].help = help;
		help->philo[i].last_time_eat = 0;
		if (i % 2)
		{
			help->philo[i].l_fork = &help->forks[i];
			help->philo[i].r_fork = &help->forks[(i + 1) % NBR_PHILO];
		}
		else
		{
			help->philo[i].r_fork = &help->forks[i];
			help->philo[i].l_fork = &help->forks[(i + 1) % NBR_PHILO];
		}
		i++;
	}
	i = -1;
	while (++i < NBR_PHILO)
		pthread_create(&help->t1[i], NULL, ss, &help->philo[i]);
	// create a one thread for monitor
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

void l7day (t_help *help)
{
	while (1)
	{
		int i = 0;
		while(i < NBR_PHILO)
		{
			pthread_mutex_lock(&help->mutex_data);
			if (get_current_time() - help->philo[i].last_time_eat > TIME_TO_DIE)
			{
				pthread_mutex_lock(&help->mutex_data);
				help->deads = 1;
				pthread_mutex_unlock(&help->mutex_data);
				pthread_mutex_unlock(&help->mutex_data);
			}
			pthread_mutex_unlock(&help->mutex_data);
			i++;
		}
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