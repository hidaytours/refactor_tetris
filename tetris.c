#include "tetris.h"

char Table[ROWS][COLUMNS] = {0};
suseconds_t timer = 400000;
int decrease = 1000;

Shape current;

const Shape shapes_array[7] = {
	{(char *[]){(char[]){0, 1, 1}, (char[]){1, 1, 0}, (char[]){0, 0, 0}}, 3},
	{(char *[]){(char[]){1, 1, 0}, (char[]){0, 1, 1}, (char[]){0, 0, 0}}, 3},
	{(char *[]){(char[]){0, 1, 0}, (char[]){1, 1, 1}, (char[]){0, 0, 0}}, 3},
	{(char *[]){(char[]){0, 0, 1}, (char[]){1, 1, 1}, (char[]){0, 0, 0}}, 3},
	{(char *[]){(char[]){1, 0, 0}, (char[]){1, 1, 1}, (char[]){0, 0, 0}}, 3},
	{(char *[]){(char[]){1, 1}, (char[]){1, 1}}, 2},
	{(char *[]){(char[]){0, 0, 0, 0}, (char[]){1, 1, 1, 1}, (char[]){0, 0, 0, 0}, (char[]){0, 0, 0, 0}}, 4}};

Shape FunctionCS(Shape shape)
{
	Shape new_shape = shape;
	char **copyshape = shape.array;
	new_shape.array = (char **)malloc(new_shape.width * sizeof(char *));
	int i, j;
	for (i = 0; i < new_shape.width; i++)
	{
		new_shape.array[i] = (char *)malloc(new_shape.width * sizeof(char));
		for (j = 0; j < new_shape.width; j++)
			new_shape.array[i][j] = copyshape[i][j];
	}
	return (new_shape);
}

void FunctionDS(Shape shape)
{
	int i;
	for (i = 0; i < shape.width; i++)
		free(shape.array[i]);
	free(shape.array);
}

bool FunctionCP(Shape shape)
{
	char **array = shape.array;
	int i, j;
	for (i = 0; i < shape.width; i++)
	{
		for (j = 0; j < shape.width; j++)
		{
			if ((shape.col + j < 0 || shape.col + j >= COLUMNS || shape.row + i >= ROWS))
			{
				if (array[i][j])
					return (false);
			}
			else if (Table[shape.row + i][shape.col + j] && array[i][j])
				return (false);
		}
	}
	return (true);
}

void FunctionRS(Shape shape)
{
	Shape temp = FunctionCS(shape);
	int i, j, k, width;
	width = shape.width;
	for (i = 0; i < width; i++)
	{
		for (j = 0, k = width - 1; j < width; j++, k--)
		{
			shape.array[i][j] = temp.array[k][i];
		}
	}
	FunctionDS(temp);
}

void FunctionPT(int score)
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
			printw("%c ", (Table[i][j] + Buffer[i][j]) ? '#' : '.');
		printw("\n");
	}
	printw("\nScore: %d\n", score);
}

static suseconds_t convert_to_micro_sec(struct timeval time)
{
	return (time.tv_sec * 1000000 + time.tv_usec);
}

bool has_to_update(struct timeval before_now)
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
	time = 1;
	timeout(1);
}

int main()
{
	srand(time(0));
	bool	is_game_on;
	int		score;
	score = 0;
	int c;
	initscr();
	struct timeval before_now;
	gettimeofday(&before_now, NULL);
	set_timeout(1);
	Shape new_shape = FunctionCS(shapes_array[rand() % 7]);
	new_shape.col = rand() % (COLUMNS - new_shape.width + 1);
	new_shape.row = 0;
	FunctionDS(current);
	current = new_shape;
	is_game_on = FunctionCP(current);
	FunctionPT(score);
	while (is_game_on)
	{
		if ((c = getch()) != ERR)
		{
			Shape temp = FunctionCS(current);
			switch (c)
			{
			case 's':
				temp.row++; // move down
				if (FunctionCP(temp))
					current.row++;
				else
				{
					int i, j;
					for (i = 0; i < current.width; i++)
					{
						for (j = 0; j < current.width; j++)
						{
							if (current.array[i][j])
								Table[current.row + i][current.col + j] = current.array[i][j];
						}
					}
					int n, m, sum, count = 0;
					for (n = 0; n < ROWS; n++)
					{
						sum = 0;
						for (m = 0; m < COLUMNS; m++)
						{
							sum += Table[n][m];
						}
						if (sum == COLUMNS)
						{
							count++;
							int l, k;
							for (k = n; k >= 1; k--)
								for (l = 0; l < COLUMNS; l++)
									Table[k][l] = Table[k - 1][l];
							for (l = 0; l < COLUMNS; l++)
								Table[k][l] = 0;
							timer -= decrease--;
						}
					}
					score += 100 * count;
					Shape new_shape = FunctionCS(shapes_array[rand() % 7]);
					new_shape.col = rand() % (COLUMNS - new_shape.width + 1);
					new_shape.row = 0;
					FunctionDS(current);
					current = new_shape;
					is_game_on = FunctionCP(current);
				}
				break;
			case 'd':
				temp.col++;
				if (FunctionCP(temp))
					current.col++;
				break;
			case 'a':
				temp.col--;
				if (FunctionCP(temp))
					current.col--;
				break;
			case 'w':
				FunctionRS(temp);
				if (FunctionCP(temp))
					FunctionRS(current);
				break;
			}
			FunctionDS(temp);
			FunctionPT(score);
		}
		if (has_to_update(before_now))
		{
			Shape temp = FunctionCS(current);
			switch ('s')
			{
			case 's':
				temp.row++;
				if (FunctionCP(temp))
					current.row++;
				else
				{
					int i, j;
					for (i = 0; i < current.width; i++)
					{
						for (j = 0; j < current.width; j++)
						{
							if (current.array[i][j])
								Table[current.row + i][current.col + j] = current.array[i][j];
						}
					}
					int n, m, sum, count = 0;
					for (n = 0; n < ROWS; n++)
					{
						sum = 0;
						for (m = 0; m < COLUMNS; m++)
						{
							sum += Table[n][m];
						}
						if (sum == COLUMNS)
						{
							count++;
							int l, k;
							for (k = n; k >= 1; k--)
								for (l = 0; l < COLUMNS; l++)
									Table[k][l] = Table[k - 1][l];
							for (l = 0; l < COLUMNS; l++)
								Table[k][l] = 0;
							timer -= decrease--;
						}
					}
					Shape new_shape = FunctionCS(shapes_array[rand() % 7]);
					new_shape.col = rand() % (COLUMNS - new_shape.width + 1);
					new_shape.row = 0;
					FunctionDS(current);
					current = new_shape;
					if (!FunctionCP(current))
						is_game_on = false;
				}
				break;
			case 'd':
				temp.col++;
				if (FunctionCP(temp))
					current.col++;
				break;
			case 'a':
				temp.col--;
				if (FunctionCP(temp))
					current.col--;
				break;
			case 'w':
				FunctionRS(temp);
				if (FunctionCP(temp))
					FunctionRS(current);
				break;
			}
			FunctionDS(temp);
			FunctionPT(score);
			gettimeofday(&before_now, NULL);
		}
	}
	FunctionDS(current);
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
