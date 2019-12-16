#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include <sys/time.h>

#define MIN_N 512
#define MAX_N 2048
#define MIN_D 4
#define MAX_D 16
#define MIN_C 2
#define MAX_C 3
#define MIN_M 1
#define MAX_M 9

int N, D, C, M[MAX_C], W[MAX_C][MAX_C];
int Color[MAX_N];

//[min, max)
int Rand(int min, int max)
{
	// x, y, z, w が乱数のシード
	static unsigned int x = 123456789, y = 3862436069, z = 521288629, w = 0;
	if (w == 0) w = (unsigned)time(NULL);
	unsigned int t = (x^(x<<11));
	x = y; y = z; z = w;
	w = (w^(w>>19))^(t^(t>>8));
	return min + (int)( (double)(max - min)*( (double)w/((double)UINT_MAX + 1) ) );
}
double DoubleRand()
{
	int r = Rand(0, INT_MAX);
	return r/(double)(INT_MAX - 1);
}

int get_elapsed_time(struct timeval *begin, struct timeval *end)
{
    return (end->tv_sec - begin->tv_sec) * 1000
	+ (end->tv_usec - begin->tv_usec) / 1000;
}

void Swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

int Max(int a, int b)
{
	if (a >= b) return a;
	return b;
}

int Min(int a, int b)
{
	if (a <= b) return a;
	return b;
}

void Input()
{
	scanf("%d%d%d", &N, &D, &C);
	for (int i = 0; i < C; i++)
	{
		scanf("%d", M + i);
	}
	for (int i = 0; i < C; i++)
	{
		for (int j = 0; j < C; j++)
		{
			scanf("%d", W[i] + j);
		}
	}
	
	int cnt = 0;
	for (int i = 0; i < C; i++)
	{
		for (int j = 0; j < M[i]; j++, cnt++)
		{
			Color[cnt] = i;
		}
	}
}