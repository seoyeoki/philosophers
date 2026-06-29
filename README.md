# Philosophers - The Dining Philosophers Problem

An implementation of the classic Dining Philosophers problem, written in C using POSIX threads (`pthread`) and mutexes. Designed and implemented in compliance with the **42 Philosophers project** requirements.

---

## 📖 Overview

The Dining Philosophers problem is a classic multi-threaded synchronization problem. It illustrates the challenges of allocating shared resources (forks) among multiple processes (philosophers) without causing **deadlock** or **starvation**.

### ⚙️ Simulation Rules
* One or more philosophers sit at a round table. There is a single large bowl of spaghetti in the middle of the table.
* The philosophers alternatively **eat**, **sleep**, or **think**. While they are eating, they are not thinking or sleeping; while sleeping, they are not eating or thinking; and, of course, while thinking, they are not eating or sleeping.
* There are also forks on the table. There are exactly as many forks as philosophers.
* A philosopher must take their right and left forks to eat.
* When a philosopher has finished eating, they put their forks back on the table and start sleeping. Once awake, they start thinking.
* The simulation stops when a philosopher dies of starvation.
* Philosophers do not speak with each other and do not know if another philosopher is about to die.

---

## 🛠️ Compilation and Usage

### Prerequisites
* GCC or Clang compiler
* POSIX thread library (`pthread`)
* Make

### Compilation
Compile the project by running:
```bash
make
```
This produces the executable named `philo`.

### Running the Simulation
Execute the compiled binary with the following arguments:
```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

* **`number_of_philosophers`**: The number of philosophers and also the number of forks.
* **`time_to_die`** (in milliseconds): If a philosopher doesn't start eating `time_to_die` milliseconds after their last meal or the start of the simulation, they die.
* **`time_to_eat`** (in milliseconds): The time it takes for a philosopher to eat (during which they hold both forks).
* **`time_to_sleep`** (in milliseconds): The time a philosopher spends sleeping.
* **`[number_of_times_each_philosopher_must_eat]`** (*Optional*): If all philosophers eat at least this many times, the simulation terminates cleanly. If not specified, the simulation runs until a philosopher dies.

### Examples
1. **Philosopher dies of starvation (immediate death)**:
   ```bash
   ./philo 1 800 200 200
   ```
2. **Philosophers survive indefinitely**:
   ```bash
   ./philo 5 800 200 200
   ```
3. **Simulation exits after all eat 7 times**:
   ```bash
   ./philo 5 800 200 200 7
   ```

---

## 🖥️ Output Format

State changes of a philosopher are printed to stdout with the following format:
* `timestamp_in_ms X has taken a fork`
* `timestamp_in_ms X is eating`
* `timestamp_in_ms X is sleeping`
* `timestamp_in_ms X is thinking`
* `timestamp_in_ms X died`

*(Where `timestamp_in_ms` is the time elapsed since the start of the simulation in milliseconds, and `X` is the philosopher's ID from 1 to N)*

---

## 💡 Key Design Highlights
* **Thread Scheduling**: Staggered startup delay ($\frac{T_{\text{cycle}}}{5}$) prevents thread contention and startup starvation under massive table counts (e.g., $N=199$).
* **Thinking Buffers**: Odd philosopher counts automatically introduce dynamic thinking margins `(1.5 * time_to_eat - time_to_sleep)` to prevent neighboring starvation when eating turns cycle.
* **Thread Safety**: All reads and writes to shared memory states (`last_meal_time`, `meals_eaten`, global stop flag) are protected symmetrically using mutex locks to completely eliminate data races.
