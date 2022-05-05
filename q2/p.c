#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <wait.h>
#include <fcntl.h>
#include <string.h>

// for printing the matrices 
void printMatrix(double array[][4], const int rows, const int cols)
{

	// the upper
	printf("\n\t  __");

	for (int i = 0; i <= cols; i++)
	{
		printf("\t ");
	}

	printf("\b\b\b __\n");

	printf("\t |");

	for (int i = 0; i <= cols; i++)
	{
		printf("\t ");
	}

	printf("\b\b  |\n");

	// the main content
	for (int i = 0; i < rows; i++)
	{
		printf("\t | ");
		for (int j = 0; j < cols; j++)
		{

			printf("%8.2f", array[i][j]);
		}

		printf("\t | \n");
	}

	// the lower 
	
	printf("\t |_");

	for (int i = 0; i <= cols; i++)
	{
		printf("\t ");
	}

	printf("\b\b _|\n\n");
}
int main(void)
{

	double number[3][4] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 11, 22};
	printMatrix(number, 3, 4);

	return 0;
}
