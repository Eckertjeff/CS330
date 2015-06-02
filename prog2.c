#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *PrintAlpha(void *alphachar);

int main(int argc, char *argv[])
{
	int i;
	pthread_t id[26];
	for (i=0; i<26; i++)
	{
	pthread_create(&id[i], NULL, PrintAlpha, (void*)i+'A');
	}
	for (i=0; i<26; i++)
	{
	pthread_join(id[i], NULL);
	}
	return EXIT_SUCCESS;
}

void *PrintAlpha(void *alphachar)
{
	int i, j;
	char letter = (char)alphachar;
	for (i=0; i < 100; i++)
	{
		printf("%c",letter);
		fflush(stdout);
		for (j=0; j<10000000; j++)
		{}//May be optimized out by the compiler, used to add delay
		  //without taking the process off the CPU.
	}
	printf("\n");
	return NULL;
}