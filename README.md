*This project has been created as part of the 42 curriculum by seoykim.*

# Philosophers - The Dining Philosophers Problem

An implementation of the classic Dining Philosophers problem, written in C using POSIX threads (`pthread`) and mutexes. Designed and implemented in compliance with the **42 Philosophers project** requirements.

---

## 📖 Description

The Dining Philosophers problem is a classic multi-threaded synchronization problem. It illustrates the challenges of allocating shared resources (forks) among multiple processes (philosophers) without causing **deadlock** or **starvation**.

### ⚙️ Simulation Rules
* One or more philosophers sit at a round table. There is a single large bowl of spaghetti in the middle of the table.
* The philosophers alternatively **eat**, **sleep**, or **think**. While they are eating, they are not thinking or sleeping; while sleeping, they are not eating or thinking; and, of course, while thinking, they are not eating or sleeping.
* There are also forks on the table. There are exactly as many forks as philosophers.
* A philosopher must take their right and left forks to eat.
* When a philosopher has finished eating, they put their forks back on the table and start sleeping. Once awake, they start thinking.
* The simulation stops when a philosopher dies of starvation.
* Philosophers do not speak with each other and do not know if another philosopher is about to die.

### 🚫 Global Rules
* Global variables are forbidden.
* The program must accept the following command-line arguments:
  * `number_of_philosophers`: The number of philosophers and also the number of forks.
  * `time_to_die` (in milliseconds): If a philosopher doesn't start eating `time_to_die` milliseconds after their last meal or the start of the simulation, they die.
  * `time_to_eat` (in milliseconds): The time it takes for a philosopher to eat (during which they hold both forks).
  * `time_to_sleep` (in milliseconds): The time a philosopher spends sleeping.
  * `[number_of_times_each_philosopher_must_eat]` (*Optional*): If all philosophers eat at least this many times, the simulation terminates cleanly.

---

## 🛠️ Instructions

### Prerequisites
* GCC or Clang compiler
* POSIX thread library (`pthread`)
* Make

### Compilation
Compile the project by navigating to the `philo/` directory and running:
```bash
cd philo
make
```
This produces the executable named `philo`.

The `Makefile` supports the following rules:
* `make` / `make all`: Compiles the source files into the `philo` executable.
* `make clean`: Removes all object files (`.o`).
* `make fclean`: Removes all object files and the executable.
* `make re`: Performs a clean rebuild (re-compilation).

### Running the Simulation
Execute the compiled binary with the following arguments:
```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

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

### Output Format
State changes of a philosopher are printed to standard output (stdout) with the following format:
* `timestamp_in_ms X has taken a fork`
* `timestamp_in_ms X is eating`
* `timestamp_in_ms X is sleeping`
* `timestamp_in_ms X is thinking`
* `timestamp_in_ms X died`

*(Where `timestamp_in_ms` is the time elapsed since the start of the simulation in milliseconds, and `X` is the philosopher's ID from 1 to N)*

---

## 💡 Key Design Highlights
* **Thread Safety & Data Race Prevention**: Symmetrical mutex locking protects all read/write accesses to shared variables (such as `last_meal_time`, `meals_eaten`, and the simulation ending flag `over`) to prevent data races.
* **Deadlock Prevention (Asymmetric Fork Acquisition)**: Philosophers acquire forks in a specific order (e.g., lower index first) to break the circular wait condition.
* **Thread Scheduling & Staggered Startup**: A dynamic initial delay is applied to philosophers based on their ID to prevent lock contention and initial starvation (critical for large tables like N=199).
* **Thinking Buffers (Odd Count Handling)**: Dynamic thinking margins `(1.5 * time_to_eat - time_to_sleep)` are calculated for odd counts of philosophers to ensure stable rotation of forks and prevent starvation.

---

## 📚 Resources

### References
* [Dining Philosophers Problem - Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
* [POSIX Threads Programming (pthread)](https://computing.llnl.gov/tutorials/pthreads/)
* [Multithreaded Programming Guide (Oracle)](https://docs.oracle.com/cd/E19455-01/806-5257/index.html)
* [Data Races and Mutex Locks](https://www.geeksforgeeks.org/mutex-lock-for-thread-synchronization-in-c/)

### AI Usage Statement
During the development of this project, an AI assistant was utilized to assist with the following tasks and components:
* **System Design & Mathematical Formulas**: Assisted in calculating the optimal startup delay (`init_delay`) and think buffer times for odd/even philosopher count configurations to avoid starvation under massive thread counts.
* **Concurrency Debugging**: Guided the design of thread synchronization mechanisms, ensuring symmetrical locking of all shared resource accesses to eliminate data races.
* **Testing Guidelines & Verification**: Assisted in drafting evaluation test cases, command cheat sheets, and setting up compilation/leak sanitizers (`-fsanitize=thread`, `-fsanitize=address`) and Valgrind tools.
* **Documentation**: Helped structure and formulate the explanation of concurrent scheduling, study notes (`학습내용.md`), and the project `README.md` according to the 42 curriculum standards.
