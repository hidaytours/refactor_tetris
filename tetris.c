#include "tetris.h"

const Block blocks_array[7] = {
	{(char *[]){(char[]){0, 1, 1}, (char[]){1, 1, 0}, (char[]){0, 0, 0}}, 3, 0, 0},
	{(char *[]){(char[]){1, 1, 0}, (char[]){0, 1, 1}, (char[]){0, 0, 0}}, 3, 0, 0},
	{(char *[]){(char[]){0, 1, 0}, (char[]){1, 1, 1}, (char[]){0, 0, 0}}, 3, 0, 0},
	{(char *[]){(char[]){0, 0, 1}, (char[]){1, 1, 1}, (char[]){0, 0, 0}}, 3, 0, 0},
	{(char *[]){(char[]){1, 0, 0}, (char[]){1, 1, 1}, (char[]){0, 0, 0}}, 3, 0, 0},
	{(char *[]){(char[]){1, 1}, (char[]){1, 1}}, 2, 0, 0},
	{(char *[]){(char[]){0, 0, 0, 0}, (char[]){1, 1, 1, 1}, (char[]){0, 0, 0, 0}, (char[]){0, 0, 0, 0}}, 4, 0, 0}};

Block copy_block(Block block)
{
	Block copied = block;
	char **array = block.array;
	copied.array = (char **)malloc(copied.width * sizeof(char *));
	int i, j;
	for (i = 0; i < copied.width; i++)
	{
		copied.array[i] = (char *)malloc(copied.width * sizeof(char));
		for (j = 0; j < copied.width; j++)
			copied.array[i][j] = array[i][j];
	}
	return (copied);
}

void free_block(Block block)
{
	int i;
	for (i = 0; i < block.width; i++)
		free(block.array[i]);
	free(block.array);
}

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

void rotate_block(Block block)
{
	Block tmp = copy_block(block);
	int i, j, k, width;
	width = block.width;
	for (i = 0; i < width; i++)
	{
		for (j = 0, k = width - 1; j < width; j++, k--)
		{
			block.array[i][j] = tmp.array[k][i];
		}
	}
	free_block(tmp);
}

void display_game(Block current, const char table[ROWS][COLUMNS], const int score)
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
	printw("\nScore: %d\n", score);
}

static suseconds_t convert_to_micro_sec(struct timeval time)
{
	return (time.tv_sec * 1000000 + time.tv_usec);
}

bool has_time_passed(struct timeval before_now, suseconds_t timer)
{
	suseconds_t sec_now;
	suseconds_t sec_before_now;
	struct timeval now;

	gettimeofday(&now, NULL);
	sec_before_now = convert_to_micro_sec(before_now);
	sec_now = convert_to_micro_sec(now);
	return ((sec_now - sec_before_now) > timer);
}

void set_timeout(int time)
{
	timeout(time);
}

void update_current(Block *current)
{
	Block new;

	new = copy_block(blocks_array[rand() % 7]);
	new.col = rand() % (COLUMNS - new.width + 1);
	new.row = 0;
	free_block(*current);
	*current = new;
}

void land_block(char (*table)[ROWS][COLUMNS], Block *current, int *score, bool *game_on, suseconds_t *timer, int *decrease)
{
	int i, j;
	for (i = 0; i < current->width; i++)
	{
		for (j = 0; j < current->width; j++)
		{
			if (current->array[i][j])
				(*table)[current->row + i][current->col + j] = current->array[i][j];
		}
	}
	int n, m, sum, count = 0;
	for (n = 0; n < ROWS; n++)
	{
		sum = 0;
		for (m = 0; m < COLUMNS; m++)
		{
			sum += (*table)[n][m];
		}
		if (sum == COLUMNS)
		{
			count++;
			int l, k;
			for (k = n; k >= 1; k--)
				for (l = 0; l < COLUMNS; l++)
					(*table)[k][l] = (*table)[k - 1][l];
			for (l = 0; l < COLUMNS; l++)
				(*table)[k][l] = 0;
			*timer -= (*decrease)--;
		}
	}
	*score += 100 * count;
	update_current(current);
	*game_on = is_game_on(*current, *table);
}

int main()
{
	suseconds_t timer = 400000;
	int decrease = 1000;
	srand(time(0));
	char Table[ROWS][COLUMNS] = {0};
	Block current;
	bool game_on;
	int score;
	score = 0;
	int c;
	initscr();
	struct timeval before_now;
	gettimeofday(&before_now, NULL);
	set_timeout(1);
	current = copy_block(blocks_array[rand() % 7]);
	game_on = is_game_on(current, Table);
	display_game(current, Table, score);
	while (game_on)
	{
		if ((c = getch()) != ERR)
		{
			Block temp = copy_block(current);
			switch (c)
			{
			case 's':
				temp.row++; // move down
				if (is_game_on(temp, Table))
					current.row++;
				else
					land_block(&Table, &current, &score, &game_on, &timer, &decrease);
				break;
			case 'd':
				temp.col++;
				if (is_game_on(temp, Table))
					current.col++;
				break;
			case 'a':
				temp.col--;
				if (is_game_on(temp, Table))
					current.col--;
				break;
			case 'w':
				rotate_block(temp);
				if (is_game_on(temp, Table))
					rotate_block(current);
				break;
			}
			free_block(temp);
			display_game(current, Table, score);
		}
		if (has_time_passed(before_now, timer))
		{
			Block temp = copy_block(current);
			temp.row++;
			if (is_game_on(temp, Table))
				current.row++;
			else
				land_block(&Table, &current, &score, &game_on, &timer, &decrease);
			free_block(temp);
			display_game(current, Table, score);
			gettimeofday(&before_now, NULL);
		}
	}
	free_block(current);
	endwin();
	int i, j;
	for (i = 0; i < ROWS; i++)
	{
		for (j = 0; j < COLUMNS; j++)
			printf("%c ", Table[i][j] ? '#' : '.');
		printf("\n");
	}
	printf("\nGame over!\n");
	printf("\nScore: %d\n", score);
	return 0;
}
