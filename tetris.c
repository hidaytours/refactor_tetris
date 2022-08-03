#include "tetris.h"

bool is_game_on(Block block, const char table[ROWS][COLUMNS])
{
	char **array = block.array;
	int i, j;
	for (i = 0; i < block.width; i++)
	{
		for (j = 0; j < block.width; j++)
		{
			if ((block.col + j < 0 || block.col + j >= COLUMNS || block.row + i >= ROWS))
			{
				if (array[i][j])
					return (false);
			}
			else if (table[block.row + i][block.col + j] && array[i][j])
				return (false);
		}
	}
	return (true);
}

void display_game(Block current, const char table[ROWS][COLUMNS])
{
	char Buffer[ROWS][COLUMNS] = {0};
	int i, j;
	for (i = 0; i < current.width; i++)
	{
		for (j = 0; j < current.width; j++)
		{
			if (current.array[i][j])
				Buffer[current.row + i][current.col + j] = current.array[i][j];
		}
	}
	clear();
	for (i = 0; i < COLUMNS - 9; i++)
		printw(" ");
	printw("42 Tetris\n");
	for (i = 0; i < ROWS; i++)
	{
		for (j = 0; j < COLUMNS; j++)
			printw("%c ", (table[i][j] + Buffer[i][j]) ? '#' : '.');
		printw("\n");
	}
	printw("\nScore: %d\n", get_total_score());
}

void	print_gameover(char table[ROWS][COLUMNS])
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLUMNS; j++)
			printf("%c ", table[i][j] ? '#' : '.');
		printf("\n");
	}
	printf("\nGame over!\n");
	printf("\nScore: %d\n", get_total_score());
}

int main()
{
	char table[ROWS][COLUMNS] = {0};
	int c;
	initscr();
	srand(time(0));
	set_timeout(1);
	save_time();
	Block current;
	current = copy_block(get_random_block());
	bool game_on = is_game_on(current, table);
	display_game(current, table);
	while (game_on)
	{
		if ((c = getch()) != ERR)
		{
			switch (c)
			{
			case 's':
				move_down(&table, &current, &game_on);
				break;
			case 'd':
				move_right(&table, &current);
				break;
			case 'a':
				move_left(&table, &current);
				break;
			case 'w':
				move_rotate(&table, &current);
				break;
			}
			display_game(current, table);
		}
		if (has_time_passed())
		{
			move_down(&table, &current, &game_on);
			display_game(current, table);
			save_time();
		}
	}
	free_block(current);
	endwin();
	print_gameover(table);
	return (0);
}
