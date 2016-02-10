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
	for (i = 0; i < 2; ++i)
    {
        for (j = 0; j < n-1; ++j)
        {
            if ((arr[j+1]->frequency) > (arr[j]->frequency))
            {
                TreeNode* a =  arr[j+1];
                arr[j+1] = arr[j];
                arr[j] = a;
            }
        }
    }
}

void createCodes(char codeWords[256][30], int depth, char currentCode[30], TreeNode* root)
{
	if (root != NULL)
	{
		if (root -> symbol != '*')
		{
			currentCode[depth] = '\0';
			sprintf(codeWords[root->symbol], "%s", currentCode);
			//codeWords[root->symbol][depth] = '\0';
			return;
		}
		currentCode[depth] = '0';
		createCodes(codeWords, depth+1, currentCode, root->left);
		currentCode[depth] = '1';
		createCodes(codeWords, depth+1, currentCode, root->right);
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
	int i, numbytes = 0, length = 0, sizeD = 7000000;
	char message[sizeD], ch;
	FILE* fin = fopen("mobydick.txt" , "r" ) ;
	FILE* out = fopen("encodeOUT.txt", "w");
	length = fread(message, sizeof(char), sizeD, fin);
	for (i = 0; i < length; i++)
		freq[message[i]]++;

	TreeNode* arr[256];
	int j = 0;

	for (i = 0; i < 256; i++)
	{
		if (freq[i] != 0)
		{
			ch = (char)i;
			t = (TreeNode*)malloc( sizeof(TreeNode) );
			//
			t -> symbol = ch ; // === t -> symbol = 'A';
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
		sortArr(arr, j);
		j--;
	}
	TreeNode* root = arr[0];
	char codeWords[256][30];
	char currentCode[30];
	createCodes(codeWords, 0, currentCode, root);
	for (i = 0; i < 256; i++)
	{
		if (freq[i] != 0)
		{
			fprintf(out, "%c", (char)i);
			fprintf(out, "%s", codeWords[(char)i]);
			fprintf(out, "\n");
		}
	}
	for (i = 0; i < length; i++)
	{
		fprintf(out, "%s", codeWords[message[i]]);
	}
	fprintf(out, "\n");
	fclose(out);
	return 0;
}
//
// end of file
//
