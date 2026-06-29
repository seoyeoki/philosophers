#include "philo.h"

static int	init_forks(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(t_fork) * data->num_philos);
	if (!data->forks)
		return (0);
	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_mutex_init(&data->forks[i].mutex, NULL) != 0)
		{
			while (i-- > 0)
				pthread_mutex_destroy(&data->forks[i].mutex);
			free(data->forks);
			data->forks = NULL;
			return (0);
		}
		i++;
	}
	return (1);
}

static int	init_philos(t_data *data)
{
	int		i;
	t_philo	*p;

	data->philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!data->philos)
		return (0);
	i = -1;
	while (++i < data->num_philos)
	{
		p = &data->philos[i];
		p->id = i + 1;
		p->meals_eaten = 0;
		p->last_meal_time = 0;
		p->thread = 0;
		p->left_fork_id = i;
		p->right_fork_id = (i + 1) % data->num_philos;
		p->data = data;
		if (pthread_mutex_init(&p->meal_mutex, NULL) != 0)
		{
			while (i-- > 0)
				pthread_mutex_destroy(&data->philos[i].meal_mutex);
			free(data->philos);
			data->philos = NULL;
			return (0);
		}
	}
	return (1);
}

int	init_dinner(t_data *data)
{
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (0);
	if (!init_forks(data) || !init_philos(data))
	{
		cleanup(data);
		return (0);
	}
	return (1);
}

void	cleanup(t_data *data)
{
	int	i;

	i = 0;
	while (data->forks && i < data->num_philos)
	{
		pthread_mutex_destroy(&data->forks[i].mutex);
		i++;
	}
	free(data->forks);
	data->forks = NULL;
	i = 0;
	while (data->philos && i < data->num_philos)
	{
		pthread_mutex_destroy(&data->philos[i].meal_mutex);
		i++;
	}
	free(data->philos);
	data->philos = NULL;
	pthread_mutex_destroy(&data->print_mutex);
}
