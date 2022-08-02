NAME = tetris
INCLUDE = ./include

CFLAGS = -Wall -Wextra -Werror -lncurses -I $(INCLUDE)

all: $(NAME)
$(NAME): tetris.c
	$(CC) $(CFLAGS) tetris.c -o tetris
