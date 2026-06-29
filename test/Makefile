NAME_USLEEP		= philo_usleep_test
NAME_NOGUARD	= philo_no_guard_test
NAME_BUSYWAIT	= philo_busy_wait_test
NAME_BUSYSPIN	= philo_busy_spin_test
NAME_NOMUTEX	= philo_no_mutex_test
NAME_EFFICIENT	= philo_efficient_test

CC				= cc
CFLAGS			= -Wall -Wextra -Werror -pthread

SRCS_COMMON		= main.c init.c
OBJS_USLEEP		= $(SRCS_COMMON:.c=.o) monitor.o routine.o utils.o
OBJS_NOGUARD	= $(SRCS_COMMON:.c=.o) monitor.o routine.o utils_no_guard.o
OBJS_BUSYWAIT	= $(SRCS_COMMON:.c=.o) monitor.o routine_busy_wait.o utils.o
OBJS_BUSYSPIN	= $(SRCS_COMMON:.c=.o) monitor.o routine.o utils_busy_spin.o
OBJS_NOMUTEX	= $(SRCS_COMMON:.c=.o) monitor_no_mutex.o routine_no_mutex.o utils_no_mutex.o
OBJS_EFFICIENT	= $(SRCS_COMMON:.c=.o) monitor.o routine_efficient.o utils.o

all: $(NAME_USLEEP) $(NAME_NOGUARD) $(NAME_BUSYWAIT) $(NAME_BUSYSPIN) $(NAME_NOMUTEX) $(NAME_EFFICIENT)

$(NAME_USLEEP): $(OBJS_USLEEP)
	$(CC) $(CFLAGS) -o $(NAME_USLEEP) $(OBJS_USLEEP)

$(NAME_NOGUARD): $(OBJS_NOGUARD)
	$(CC) $(CFLAGS) -o $(NAME_NOGUARD) $(OBJS_NOGUARD)

$(NAME_BUSYWAIT): $(OBJS_BUSYWAIT)
	$(CC) $(CFLAGS) -o $(NAME_BUSYWAIT) $(OBJS_BUSYWAIT)

$(NAME_BUSYSPIN): $(OBJS_BUSYSPIN)
	$(CC) $(CFLAGS) -o $(NAME_BUSYSPIN) $(OBJS_BUSYSPIN)

$(NAME_NOMUTEX): $(OBJS_NOMUTEX)
	$(CC) $(CFLAGS) -o $(NAME_NOMUTEX) $(OBJS_NOMUTEX)

$(NAME_EFFICIENT): $(OBJS_EFFICIENT)
	$(CC) $(CFLAGS) -o $(NAME_EFFICIENT) $(OBJS_EFFICIENT)

%.o: %.c philo.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SRCS_COMMON:.c=.o) monitor.o monitor_no_mutex.o routine.o routine_busy_wait.o routine_no_mutex.o routine_efficient.o utils.o utils_no_guard.o utils_busy_spin.o utils_no_mutex.o

fclean: clean
	rm -f $(NAME_USLEEP) $(NAME_NOGUARD) $(NAME_BUSYWAIT) $(NAME_BUSYSPIN) $(NAME_NOMUTEX) $(NAME_EFFICIENT)

re: fclean all

.PHONY: all clean fclean re
