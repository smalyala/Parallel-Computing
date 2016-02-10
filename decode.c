//
// Torbert, 4 Sept 2015
//
#include <stdio.h>
#include <math.h>
//
int main()
{
	FILE* fin = fopen( "encodeOUT.txt" , "r" ) ;
	//
	char ch, symb ;
	//
	int n = 0 ;
	//M
	char tree[7000000];
	int numbytes;
	int frequency[256] = {0};
	int numLines = 80;
	int i, k;
	double countA, countB;

	for (i = 0; i < (sizeof(tree)/sizeof(char)); i++)
	{
		tree[i] = '\0';
	}
	//
	for (i = 0; i < numLines; i++)
	{
		fread( &symb , sizeof(char) , 1 , fin ) ;
		k = 1;
		while( 1 )
		{
			fread( &ch , sizeof(char) , 1 , fin ) ;
			//
			if (ch == '\n')
			{
				tree[k] = symb;
				break;
			}
			else if (ch == '0') k = 2*k;
			else k = 2*k+1;
			//
		}
	}
	k = 1;
	countA = 0.0;
	countB = 0.0;
	while (1) 
	{
		fread(&ch, sizeof(char), 1, fin);
		if (ch == '0') k = 2*k;
		else if (ch == '1') k = 2*k+1;
		else break;
		if (tree[k] != '\0')
		{
			printf("%c", tree[k]);
			countA++;
			frequency[(int)tree[k]]++;
			k = 1;
		}
		countB++;
	}
	double shannon = 0.0;
	for (i = 0; i < 256; i++)
	{
		if (frequency[i] != 0)
		{
			shannon += frequency[i] * (-log2(frequency[i]/countA));
		}
	}
	printf("\n");
	printf("%G\n", ((countA*8)-countB)/(countA*8));
	printf("%d\n", (int)round(shannon));
	return 0;
	fclose( fin ) ;
}
//
// end of file
//
