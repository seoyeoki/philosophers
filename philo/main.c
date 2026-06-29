#include "philo.h"

static int	ft_isdigit(char c)
{
	return (c >= '0' && c <= '9');
}

static int  parse_int(const char *str, int *n)
{
    long    val;
    int i;

    i = 0;
    val = 0;
    if (!str || !str[0])
        return (0);
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return (0);
        val = val * 10 + str[i] - '0';
        if (val > 2147483647)
            return (0);
        i++;
    }
    *n = (int)val;
    return (1);
}

static int parse_args(t_data *data, int argc, char **argv) {
  int val;

  data->must_eat = -1;
  if (!parse_int(argv[1], &val) || val <= 0)
    return (0);
  data->num_philos = val;
  if (!parse_int(argv[2], &val) || val < 0)
    return (0);
  data->time_to_die = val;
  if (!parse_int(argv[3], &val) || val < 0)
    return (0);
  data->time_to_eat = val;
  if (!parse_int(argv[4], &val) || val < 0)
    return (0);
  data->time_to_sleep = val;
  if (argc == 6) {
    if (!parse_int(argv[5], &val) || val <= 0)
      return (0);
    data->must_eat = val;
  }
  return (1);
}

int main(int argc, char **argv) {
  t_data data;

  if (argc < 5 || argc > 6) {
    write(2, "Usage: ./philo n_philos n_die n_eat n_sleep [n_must_eat]\n", 55);
    return (1);
  }
  memset(&data, 0, sizeof(t_data));
  if (!parse_args(&data, argc, argv)) {
    write(2, "Error: Invalid Arguments\n", 25);
    return (1);
  }
  if (!init_dinner(&data)) {
    write(2, "Error: Initialization failed\n", 30);
    return (1);
  }
  start_dinner(&data);
  cleanup(&data);
  return (0);
}