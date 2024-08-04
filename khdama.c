#include <fcntl.h>
#include <limits.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

#define NBR_PHILO 5
#define TIME_TO_DIE 179
#define TIME_TO_EAT 60
#define TIME_TO_SLEEP 60

struct s_help;
typedef struct s_philo
{
	int				id;
	size_t			last_time_eat;
	size_t			meals;
	pthread_t		t1;
	struct s_help	*help;
}					t_philo;

typedef struct s_help
{
	sem_t			*sem_forks;
	sem_t			*sem_write;
	sem_t			*sem_monitor;
	sem_t			*sem_eat;
	size_t			 start;
	pid_t			*pids;
	struct s_philo	*philo;
}					t_help;


#define COLOR_RESET "\033[0m"
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"


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
	return (0);ft_msleep(1);
}

// not know
void	*monitor_die(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		sem_wait(philo->help->sem_monitor);
		// printf("BEFORE : %zu > %d\n",get_current_time() - philo->last_time_eat, TIME_TO_DIE);
		if (get_current_time() - philo->last_time_eat > (size_t)TIME_TO_DIE)
		{
		// printf("AFTER :%zu > %d\n",get_current_time() - philo->last_time_eat, TIME_TO_DIE);
			sem_wait(philo->help->sem_write);
			printf(COLOR_RED "%zu %d died\n" COLOR_RESET, get_current_time() - philo->help->start, philo->id);
			sem_post(philo->help->sem_write);
			exit(1);
		}
		sem_post(philo->help->sem_monitor);
	}
	return (NULL);
}

void routine(t_philo *philo)
{
	if (philo->id % 2 == 0)
		usleep(50);
	while (1)
	{
		sem_wait(philo->help->sem_forks);
			sem_wait(philo->help->sem_write);
			printf(COLOR_YELLOW "%zu %d has taken a fork\n" COLOR_RESET, get_current_time()
			- philo->help->start, philo->id);
			sem_post(philo->help->sem_write);

		sem_wait(philo->help->sem_forks);
			sem_wait(philo->help->sem_write);
			printf(COLOR_YELLOW "%zu %d has taken a fork\n" COLOR_RESET, get_current_time()
			- philo->help->start, philo->id);
			sem_post(philo->help->sem_write);

		sem_wait(philo->help->sem_write);
		printf(COLOR_GREEN "%zu %d is eating\n" COLOR_RESET, get_current_time()
		- philo->help->start, philo->id);
		sem_post(philo->help->sem_write);
		
		sem_wait(philo->help->sem_eat);
		philo->last_time_eat = get_current_time();
		sem_post(philo->help->sem_eat);
		ft_msleep(TIME_TO_EAT);

		sem_post(philo->help->sem_forks);
		sem_post(philo->help->sem_forks);

		sem_wait(philo->help->sem_write);
		printf(COLOR_BLUE "%zu %d is sleeping\n" COLOR_RESET, get_current_time()
			- philo->help->start, philo->id);
		sem_post(philo->help->sem_write);
		ft_msleep(TIME_TO_SLEEP);

		sem_wait(philo->help->sem_write);
		printf(COLOR_YELLOW "%zu %d is thinking\n" COLOR_RESET, get_current_time()
			- philo->help->start, philo->id);
		sem_post(philo->help->sem_write);
        ft_msleep(1);

	}
}

int	main(void)
{
	t_help	*help;
	int		i;

	help = malloc(sizeof(t_help));
	help->philo = malloc(NBR_PHILO * sizeof(t_philo));
	help->pids = malloc (NBR_PHILO * sizeof(pid_t));
    sem_unlink("forks");
	help->sem_forks = sem_open("forks", O_CREAT | O_EXCL, 0644, NBR_PHILO);
	sem_unlink("write");
	help->sem_write = sem_open("write", O_CREAT | O_EXCL, 0644, 1);
	sem_unlink("eat");
	help->sem_eat = sem_open("eat", O_CREAT | O_EXCL, 0644, 1);
	sem_unlink("monitor");
	help->sem_monitor = sem_open("monitor", O_CREAT | O_EXCL, 0644, 1);
	if (help->sem_forks == SEM_FAILED)
	{
		perror("sem_open");
		exit(EXIT_FAILURE);
	}
	help->start = get_current_time();
	i = 0;
	while (i < NBR_PHILO)
	{
		help->philo[i].id = i + 1;
		help->philo[i].help = help;
		i++;
	}
	i = 0;
	while (i < NBR_PHILO)
	{
		help->philo[i].last_time_eat = get_current_time();
		help->pids[i] = fork();
		if (help->pids[i] == 0)
		{
			pthread_create(&help->philo[i].t1, NULL, monitor_die, &help->philo[i]);
			routine(&help->philo[i]);
		}
		i++;
	}
	i = 0;
	while (i < NBR_PHILO)
	{
		pthread_join(help->philo[i].t1, NULL);
		i++;
	}
	i = 0;
	int status = 0;
	while (i < NBR_PHILO)
	{
		waitpid(-1, &status, 0);
		if (status != 0)
		{
			i = 0;
			while (i < NBR_PHILO)
			{
				kill(help->pids[i], 15);
				i++;
			}
		}
		i++;
	}
}
