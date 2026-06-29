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
	return (data->dead);
}

int	print_state(t_philo *philo, const char *state)
{
	long long	ts;

	ts = get_time() - philo->data->start_time;
	printf("%lld %d %s\n", ts, philo->id, state);
	return (1);
}
