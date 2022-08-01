NAME = tetris
INCLUDE = ./include

CFLAGS = -lncurses -I $(INCLUDE)

all: $(NAME)
$(NAME): tetris.c
	$(CC) $(CFLAGS) tetris.c -o tetris
