// MPI Demo
//    mpicc mpiDemo.c
//    mpirun -np 4 a.out
// 
// Manager-Worker model for parallel processing.
// 
// time ... real ... user 
// 
// htop 

#include <stdio.h>
#include <stdlib.h>
//
#include <time.h>
#include "mpi.h"
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
	int        rank   ;
    int        size   ;
    MPI_Status status ;
    int        tag = 0;
    //
    // other variables
    //
    int        kmpi , jmpi  ;
    //
    // boilerplate
    //
    MPI_Init(      &argc          , &argv ) ;
    MPI_Comm_size( MPI_COMM_WORLD , &size ) ; // same
    MPI_Comm_rank( MPI_COMM_WORLD , &rank ) ; // different
    //
    // manager has rank = 0
    //
    clock_t begin, end;
	srand(time(NULL));
	double probability = 0.0, ret, loopProbabilities[100], tempProb[size];
	int i, j, count, k, trials = 1000, assign, gridSize = 30;
	char forest[gridSize][gridSize];
	FILE* out = fopen("forestFireData.txt", "w");
	FILE* outTime = fopen("runTimeData.txt", "w");
	begin = clock();
	if (rank == 0) 
	{
		for (assign = 1; assign < size; assign++)
		{
			MPI_Send( &probability , 1 , MPI_DOUBLE , assign , tag , MPI_COMM_WORLD ) ;
			probability += .01;
			tempProb[assign] = probability;
		}
		while (probability < 1.0)
		{
			MPI_Recv( &ret , 1 , MPI_DOUBLE , MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status ) ;
			assign = status.MPI_SOURCE ;
			loopProbabilities[(int)(100*tempProb[assign])] = ret;
			MPI_Send( &probability , 1 , MPI_DOUBLE , assign , tag , MPI_COMM_WORLD ) ;
			tempProb[assign] = probability;
			probability += .01;
		}

		probability = -1.0;
		for (jmpi = 1; jmpi < size; jmpi++)
		{
			MPI_Recv( &ret , 1 , MPI_DOUBLE , MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status ) ;
			assign = status.MPI_SOURCE;
			loopProbabilities[(int)(100*tempProb[assign])] = ret;
			MPI_Send( &probability , 1 , MPI_DOUBLE , assign , tag , MPI_COMM_WORLD ) ;
		}
		
		for (assign = 0; assign < 100; assign++)
		{
			fprintf(out, "%f\n", loopProbabilities[assign]);
		}
		
	}
	else
	{
		while (1)
		{
			MPI_Recv( &probability , 1 , MPI_DOUBLE , 0 , tag , MPI_COMM_WORLD , &status ) ;
			if (probability < 0.0) break;
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
					//usleep(80000);
					count = count + 1;
				}
			}
			ret = count/(trials*(float)gridSize);
			MPI_Send( &ret , 1 , MPI_DOUBLE , 0 , tag , MPI_COMM_WORLD ) ;
		}
	}
	MPI_Finalize() ;
	//
	fclose(out);
	end = clock();
	//fprintf(outTime, "%f\n", (double)(end-begin)/ CLOCKS_PER_SEC);
	printf("%f\n", (double)(end-begin)/ CLOCKS_PER_SEC);
	fclose(outTime);
	printf();
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