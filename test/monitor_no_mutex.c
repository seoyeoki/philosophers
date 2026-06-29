#include "philo.h"

static int	check_death(t_data *data)
{
	int			i;
	long long	elapsed;
	long long	ts;

	i = 0;
	while (i < data->num_philos)
	{
		// No meal_mutex lock
		elapsed = get_time() - data->philos[i].last_meal_time;
		if (elapsed > data->time_to_die)
		{
			ts = get_time() - data->start_time;
			// No print_mutex lock
			printf("%lld %d died\n", ts, data->philos[i].id);
			data->dead = 1;
			return (1);
		}
		i++;
	}
	return (0);
}

static int	check_all_ate(t_data *data)
{
	int	i;
	int	count;

	if (data->must_eat < 0)
		return (0);
	i = 0;
	count = 0;
	while (i < data->num_philos)
	{
		// No meal_mutex lock
		if (data->philos[i].meals_eaten >= data->must_eat)
			count++;
		i++;
	}
	if (count == data->num_philos)
	{
		// No print_mutex lock
		data->dead = 1;
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (1)
	{
		if (check_death(data) || check_all_ate(data))
			return (NULL);
		usleep(1000);
	}
}

int	run_simulation(t_data *data)
{
	pthread_t	monitor;
	int			i;

	data->start_time = get_time();
	i = 0;
	while (i < data->num_philos)
	{
		data->philos[i].last_meal_time = data->start_time;
		if (pthread_create(&data->philos[i].thread, NULL,
				philo_routine, &data->philos[i]) != 0)
			return (0);
		i++;
	}
	if (pthread_create(&monitor, NULL, monitor_routine, data) != 0)
		return (0);
	pthread_join(monitor, NULL);
	i = 0;
	while (i < data->num_philos)
		pthread_join(data->philos[i++].thread, NULL);
	return (1);
}
