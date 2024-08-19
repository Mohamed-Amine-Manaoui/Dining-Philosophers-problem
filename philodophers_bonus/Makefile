CC = cc

CFLAGS = -Wall -Werror -Wextra #-g3 -fsanitize=thread

SRCB = inisialization_bonus.c monitor_bonus.c philo_bonus.c routine_bonus.c parsing_bonus.c external_function.c

OBJB = $(SRCB:.c=.o)

NAME = philo_bonus

all : $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJB)
	$(CC) $(CFLAGS) $(OBJB) -o $(NAME)

clean:
	rm -rf $(OBJB)

fclean: clean
	rm -rf $(NAME)
re: fclean all clean
