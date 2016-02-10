#include <stdio.h>
#include <stdlib.h>
//
#include <time.h>
//

void display(char forest[30][30], int gridSize)
{
	printf("\033[2J\033[1;1H");
	int i, j;
	for (i = 0; i < gridSize; i++)
	{
		for (j = 0; j < gridSize; j++)
		{
			if (forest[i][j] == 'T')
				printf("\x1b[32m%c\x1b[0m ", forest[i][j]);
			else if (forest[i][j] == '-')
				printf("\x1b[33m%c\x1b[0m ", forest[i][j]);
			else
				printf("\x1b[31m%c\x1b[0m ", forest[i][j]);
		}
		printf("\n");
	}
}

int hasFire(char forest[30][30], int gridSize)
{
	int i, j;
	for (i = 0; i < gridSize; i++)
	{
		for (j = 0; j < gridSize; j++)
		{
			if (forest[i][j] == 'X')
				return 1;
		}
	}
	return 0;
}

//
int main( int argc , char* argv[] )
{
	srand(time(NULL));
	double probability, ret;
	int gridSize = 30;
	char forest[gridSize][gridSize];
	int i, j, count, k, trials = 10;
	FILE* out = fopen("forestFireData.txt", "w");
	for (probability = 0.0; probability < 1.0; probability += .1)
	{
		count = 0;
		for (k = 0; k < trials; k++)
		{
			for (i = 0; i < gridSize; i++)
			{
				for (j = 0; j < gridSize; j++)
				{
					if (rand()/(float)(RAND_MAX) < probability)
					{
						forest[i][j] = 'T';
					}
					else
					{
						forest[i][j] = '-';
					}
				}
			}
			for (i = 0; i < gridSize; i++)
			{
				forest[i][0] = 'X';
			}
			//display(forest, gridSize);
			while (hasFire(forest, gridSize))
			{
				for (i = 0; i < gridSize; i++)
				{
					for (j = 0; j < gridSize; j++)
					{
						if (forest[i][j] == 'X')
						{
							forest[i][j] = '-';
							if (j > 0 && forest[i][j-1] == 'T')
							{
								forest[i][j-1] = 'x';
							}
							if (j < gridSize-1 && forest[i][j+1] == 'T')
							{
								forest[i][j+1] = 'x';
							}
							if (i > 0 && forest[i-1][j] == 'T')
							{
								forest[i-1][j] = 'x';
							}
							if (i < gridSize-1 && forest[i+1][j] == 'T')
							{
								forest[i+1][j] = 'x';
							}
						}
					}
				}
				for (i = 0; i < gridSize; i++)
				{
					for (j = 0; j < gridSize; j++)
					{
						if (forest[i][j] == 'x')
							forest[i][j] = 'X';
					}
				}
				//display(forest, gridSize);
				//usleep(500000);
				count = count + 1;
			}
		}
		fprintf(out, "%f\n", count/(trials*(float)gridSize));
	}
	//
	fclose(out);

	return 0;
}
//
// end of file
//
// time ./a.out
// Time: 0.3139879703521729 seconds
// 
// real    0m0.315s
// user    0m0.313s
// sys     0m0.001s