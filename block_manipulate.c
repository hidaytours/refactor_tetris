#include "tetris.h"

void update_current(Block *current)
{
	Block new;

	new = copy_block(get_random_block());
	new.col = rand() % (COLUMNS - new.width + 1);
	new.row = 0;
	free_block(*current);
	*current = new;
}

void fix_block(char (*table)[ROWS][COLUMNS], Block *current)
{
	for (int i = 0; i < current->width; i++)
		for (int j = 0; j < current->width; j++)
			if (current->array[i][j])
				(*table)[current->row + i][current->col + j] = current->array[i][j];
	for (int i = 0; i < ROWS; i++)
	{
		int sum = 0;
		for (int j = 0; j < COLUMNS; j++)
			sum += (*table)[i][j];
		if (sum == COLUMNS)
		{
			add_score(SCORE_PER_ROW);
			for (int k = i; k >= 1; k--)
				for (int l = 0; l < COLUMNS; l++)
					(*table)[k][l] = (*table)[k - 1][l];
			for (int l = 0; l < COLUMNS; l++)
				(*table)[0][l] = 0;
			update_threshold_timer();
		}
	}
	update_current(current);
}

void move_down(char (*table)[ROWS][COLUMNS], Block *current, bool *game_on)
{
	Block temp = copy_block(*current);
	temp.row++;
	if (is_game_on(temp, *table))
		current->row++;
	else
	{
		fix_block(table, current);
		*game_on = is_game_on(*current, *table);
	}
	free_block(temp);
}

void move_right(char (*table)[ROWS][COLUMNS], Block *current)
{
	Block temp = copy_block(*current);
	temp.col++;
	if (is_game_on(temp, *table))
		current->col++;
	free_block(temp);
}

void move_left(char (*table)[ROWS][COLUMNS], Block *current)
{
	Block temp = copy_block(*current);
	temp.col--;
	if (is_game_on(temp, *table))
		current->col--;
	free_block(temp);
}

void move_rotate(char (*table)[ROWS][COLUMNS], Block *current)
{
	Block temp = copy_block(*current);
	rotate_block(temp);
	if (is_game_on(temp, *table))
		rotate_block(*current);
	free_block(temp);
}
