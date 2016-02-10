//
// Torbert, 16 Sept 2015
//
#include <stdio.h>
#include <stdlib.h>
//
typedef struct Node
{
	char symbol ;
	//
	int frequency ;
	//
	struct Node* left ;
	struct Node* right ;
	//
} TreeNode ;

void sortArr(TreeNode* arr[], int n)
{
	int i, j;
	for (i = 0; i < n; ++i)
    {
        for (j = i+1; j < n; ++j)
        {
            if ((arr[i]->frequency) < (arr[j]->frequency))
            {
                TreeNode* a =  arr[i];
                arr[i] = arr[j];
                arr[j] = a;
            }
        }
    }
}

//
int main( int argc , char* argv[] )
{
	//
	TreeNode* t = (TreeNode*)malloc( sizeof(TreeNode) );
	//
	(*t).symbol = 'A' ; // === t -> symbol = 'A';
	t -> frequency = 7 ;
	t -> left = NULL ;
	t -> right = NULL ;
	//
	//printf( "%c\n" , t->symbol ) ;
	//printf( "%d\n" , t->frequency ) ;
	//
	int freq[256] = {0};
	int numbytes = 0;
	char ch;
	FILE* fin = fopen("encodeIN.txt" , "r" ) ;
	FILE* out = fopen("encodeOUT.txt", "w");
	while (1)
	{
		numbytes = fread(&ch, sizeof(char), 1, fin);
		if (numbytes == 0) break;
		freq[(int)ch]++;
	}
	TreeNode* arr[256];
	int i;
	int j = 0;

	for (i = 0; i < 256; i++)
	{
		if (freq[i] != 0)
		{
			ch = (char)i;
			t = (TreeNode*)malloc( sizeof(TreeNode) );
			//
			(*t).symbol = ch ; // === t -> symbol = 'A';
			t -> frequency = freq[i] ;
			t -> left = NULL ;
			t -> right = NULL ;
			arr[j] = t;
			j++;
		}
	}
	sortArr(arr, j);
	j--;
	while (j > 0)
	{
		t = (TreeNode*)malloc( sizeof(TreeNode) );
		t -> symbol = '*' ;
		t -> frequency = (arr[j]->frequency + arr[j-1]->frequency) ;
		t -> left = arr[j-1] ;
		t -> right = arr[j] ;
		arr[j-1] = t;
		j--;
		sortArr(arr, j);

	}
	printf( "%s\n", "outside");
	
	


	fclose(fin);
	return 0;
}
//
// end of file
//