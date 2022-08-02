#ifndef TETRIS_H
# define TETRIS_H

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <time.h>
# include <sys/time.h>
# include <ncurses.h>

# define ROWS 20
# define COLUMNS 15

typedef struct {
	char	**array;
	int 	width;
	int 	row;
	int		col;
}	Block;

#endif