#include <windows.h>
#include <time.h>

float rnd(int range)
{
	float r;

	r=float (rand()%range);
	return float(r);
}

void seedrnd(void)
{
	srand((unsigned)time(NULL));
}
