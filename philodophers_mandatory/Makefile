CC = cc

CFLAGS = -Wall -Werror -Wextra -pthread #-g3 -fsanitize=thread


SRC = external_function.c initialization.c monitor.c parsing.c philo.c routine_norms.c routine.c

OBJ = $(SRC:.c=.o)

NAME = philo

all : $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)
re: fclean all clean
