#include "tetris.h"

struct timeval	before_now;
suseconds_t		g_threshold_timer = 400000;
int				g_decrease = 1000;

static	suseconds_t convert_to_micro_sec(struct timeval time)
{
	return (time.tv_sec * 1000000 + time.tv_usec);
}

bool	has_time_passed()
{
	suseconds_t		sec_now;
	suseconds_t		sec_before_now;
	struct timeval	now;

	gettimeofday(&now, NULL);
	sec_before_now = convert_to_micro_sec(before_now);
	sec_now = convert_to_micro_sec(now);
	return ((sec_now - sec_before_now) > g_threshold_timer);
}

void	save_time()
{
	gettimeofday(&before_now, NULL);
}

void	update_threshold_timer()
{
	g_threshold_timer -= g_decrease--;
}

void	set_timeout(int time)
{
	timeout(time);
}