NAME_USLEEP		= philo_usleep_test
NAME_NOGUARD	= philo_no_guard_test
NAME_BUSYWAIT	= philo_busy_wait_test
NAME_BUSYSPIN	= philo_busy_spin_test

CC				= cc
CFLAGS			= -Wall -Wextra -Werror -pthread

SRCS_COMMON		= main.c init.c monitor.c
OBJS_USLEEP		= $(SRCS_COMMON:.c=.o) routine.o utils.o
OBJS_NOGUARD	= $(SRCS_COMMON:.c=.o) routine.o utils_no_guard.o
OBJS_BUSYWAIT	= $(SRCS_COMMON:.c=.o) routine_busy_wait.o utils.o
OBJS_BUSYSPIN	= $(SRCS_COMMON:.c=.o) routine.o utils_busy_spin.o

all: $(NAME_USLEEP) $(NAME_NOGUARD) $(NAME_BUSYWAIT) $(NAME_BUSYSPIN)

$(NAME_USLEEP): $(OBJS_USLEEP)
	$(CC) $(CFLAGS) -o $(NAME_USLEEP) $(OBJS_USLEEP)

$(NAME_NOGUARD): $(OBJS_NOGUARD)
	$(CC) $(CFLAGS) -o $(NAME_NOGUARD) $(OBJS_NOGUARD)

$(NAME_BUSYWAIT): $(OBJS_BUSYWAIT)
	$(CC) $(CFLAGS) -o $(NAME_BUSYWAIT) $(OBJS_BUSYWAIT)

$(NAME_BUSYSPIN): $(OBJS_BUSYSPIN)
	$(CC) $(CFLAGS) -o $(NAME_BUSYSPIN) $(OBJS_BUSYSPIN)

%.o: %.c philo.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SRCS_COMMON:.c=.o) routine.o routine_busy_wait.o utils.o utils_no_guard.o utils_busy_spin.o

fclean: clean
	rm -f $(NAME_USLEEP) $(NAME_NOGUARD) $(NAME_BUSYWAIT) $(NAME_BUSYSPIN)

re: fclean all

.PHONY: all clean fclean re
