.DEFAULT_GOAL = all

NAME = monitor_open_fd

CC = gcc
CFLAGS = -Wall -Werror -Wextra

$(NAME):
	$(CC) $(CFLAGS) src/main.c -o $(NAME)

all: $(NAME)

clean:
	rm -f $(NAME)

re: clean all

.PHONY: all clean re