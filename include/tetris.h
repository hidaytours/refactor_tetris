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

# define SCORE_PER_ROW 100

typedef struct {
	char	**array;
	int 	width;
	int 	row;
	int		col;
}	Block;

Block get_random_block();

bool is_game_on(Block block, const char table[ROWS][COLUMNS]);

Block copy_block(Block block);
void free_block(Block block);
void rotate_block(Block block);

void save_time();
bool has_time_passed();
void set_timeout(int time);
void update_threshold_timer();

void	add_score(int score);
int get_total_score();

void move_down(char (*table)[ROWS][COLUMNS], Block *current, bool *game_on);
void move_right(char (*table)[ROWS][COLUMNS], Block *current);
void move_left(char (*table)[ROWS][COLUMNS], Block *current);
void move_rotate(char (*table)[ROWS][COLUMNS], Block *current);

#endif