*This project has been created as part of the 42 curriculum by seoyeon.*

# Philosophers

## Description
This project is an implementation of the famous **Dining Philosophers Problem**, a classic multi-threading synchronization problem. The goal is to simulate philosophers sitting at a round table, doing three activities: eating, sleeping, and thinking. 

There are forks on the table, with exactly one fork between each pair of philosophers. To eat, a philosopher must acquire both their left and right forks. The simulation stops immediately if a philosopher dies of starvation (i.e., if they do not start eating within `time_to_die` milliseconds since the start of their last meal or the simulation).

This implementation uses **threads** to represent each philosopher and **mutexes** to protect the state of each fork, ensuring a data-race-free and deadlock-free execution.

## Instructions

### Compilation
To compile the project, run the following command inside the `philo/` directory:
```bash
make
```
This will compile the source files with `-Wall -Wextra -Werror` flags and produce the executable `philo`.

### Execution
Run the program with the following arguments:
```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

- `number_of_philosophers`: The number of philosophers and also the number of forks.
- `time_to_die` (in milliseconds): If a philosopher has not started eating within this time since their last meal or the start of the simulation, they die.
- `time_to_eat` (in milliseconds): The time it takes for a philosopher to eat.
- `time_to_sleep` (in milliseconds): The time a philosopher spends sleeping.
- `[number_of_times_each_philosopher_must_eat]` (optional): If all philosophers have eaten at least this many times, the simulation stops cleanly. If not specified, the simulation stops when a philosopher dies.

#### Example
```bash
./philo 5 800 200 200
```

## Resources
- [POSIX Threads (pthreads) Documentation](https://man7.org/linux/man-pages/man7/pthreads.7.html)
- [Dining Philosophers Problem - Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- **AI Usage**: AI was used during this project to review codebase compliance, verify thread safety, run performance audits with Thread Sanitizer, and check for memory leaks.
