#include "philo.h"

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	ft_usleep(long long ms)
{
	long long	start;

	start = get_time();
	while (get_time() - start < ms)
		usleep(500);
}

int	is_dead(t_data *data)
{
	int	dead;

	pthread_mutex_lock(&data->print_mutex);
	dead = data->dead;
	pthread_mutex_unlock(&data->print_mutex);
	return (dead);
}

int	print_state(t_philo *philo, const char *state)
{
	long long	ts;

	pthread_mutex_lock(&philo->data->print_mutex);
	if (philo->data->dead)
	{
		pthread_mutex_unlock(&philo->data->print_mutex);
		return (0);
	}
	ts = get_time() - philo->data->start_time;
	printf("%lld %d %s\n", ts, philo->id, state);
	pthread_mutex_unlock(&philo->data->print_mutex);
	return (1);
}
