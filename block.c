#include "tetris.h"

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