#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#define NBR_PHILO 200
# define TIME_TO_EAT 200
# define TIME_TO_SLEEP 200
# define NBR_OF_x7al_khasso_yakl 10

struct s_help;
typedef struct s_philo
{
	int				id;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
	struct s_help *help;
	
}					t_philo;

typedef struct s_help
{
	pthread_t		*t1;
	pthread_mutex_t	*forks;
	struct s_philo	*philo;
	int nbr_of_wajba;
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
	// printf("%zu\n", philo->help->start);
	philo = (t_philo *)arg;
	while (1)
	{
		pthread_mutex_lock(philo->l_fork);
		//  printf(COLOR_YELLOW "philo %d Take a left fork !!\n" COLOR_RESET, philo->id);
        pthread_mutex_lock(philo->r_fork);
        printf(COLOR_BLUE "%zu %d Take a fork !!\n" COLOR_RESET, get_current_time() - philo->help->start , philo->id);
        printf(COLOR_GREEN "%zu %d EATING\n" COLOR_RESET, get_current_time() - philo->help->start , philo->id);
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
}

void	init_philo(t_help *help)
{
	int	i;

	i = 0;
	// init threads
	help->t1 = malloc(NBR_PHILO * sizeof(pthread_t));
	// init struct philo
	help->philo = malloc(NBR_PHILO * sizeof(t_philo));
	while (i < NBR_PHILO)
	{
		help->philo[i].id = i + 1;
		help->philo[i].help = help;
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

int	main(void)
{
	t_help	*help;
	
	help = malloc(sizeof(t_help));
	help->start = get_current_time();
	// printf(" start : %zu\n", help->start);
	init_mutex(help);
	init_philo(help);
	join_philo(help);
	return (0);
}

// philo ==> thread
// forks ==> mutex