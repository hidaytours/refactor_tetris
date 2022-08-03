#include "tetris.h"

int g_total_score = 0;

void	add_score(int score)
{
	g_total_score += score;
}

int	get_total_score()
{
	return (g_total_score);
}