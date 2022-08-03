NAME = tetris
INCLUDE = ./include

CFLAGS = -Wall -Wextra -Werror -lncurses -I $(INCLUDE)

SRCS = tetris.c time.c block.c score.c block_manipulate.c block_template.c

all: $(NAME)
$(NAME): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o tetris
