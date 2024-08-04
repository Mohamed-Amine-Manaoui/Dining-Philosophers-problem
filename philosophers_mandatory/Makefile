CC = cc

CFLAGS = -Wall -Werror -Wextra

SRC = ${shell find  -type f -name "*.c"}

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