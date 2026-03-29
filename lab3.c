//Matthew Gutierrez
//1002333697
//Lab 3

#include <stdio.h>
#include <stdlib.h>

int main (int argc, char*argv[]) 
{
	int numbers = 0;
	int eger = 0;
	int *num = (int *)malloc(numbers * sizeof(int));
	while(scanf("%d", &eger) == 1) 
	{
		numbers++;
		num = (int *)realloc(num, numbers * sizeof(int));
		num[numbers-1] = eger;
		//printf("%d",numbers);
	}
	int total = 0;
	for(int i = 0; i < numbers; i++) 
	{
		total = total + num[i];
	}
	int subseq = total/3;
	
	printf("%d\n" , total);
	free(num);
	//printf("Hello world!\n");
	return 0;
}
