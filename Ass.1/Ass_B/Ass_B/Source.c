#include<stdio.h>
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include <time.h> 

// size of array
#define MAX 15000000
int a[MAX];

int main()
{
	double time_spent = 0.0;

	clock_t begin = clock();

	for (int i = 0; i < MAX; i++)
		a[i] = rand() % 100;
	int sum = 0;
	for (int i = 0; i < MAX; i++)
	{
		sum += a[i];
	}
	printf("Total Sum : \n ");
	printf("%d", sum);

	clock_t end = clock();

	time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
	printf("\n The elapsed time is %f seconds", time_spent);

	/*IF U WANT TO TRY LOW SIZE ARRAYS
	printf("Array : \n ");
	for (int i = 0; i < MAX; i++)
		printf("\n %d \n", a[i]);
	*/
	return 0;
}



