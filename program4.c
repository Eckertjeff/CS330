#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int frameexists(int*, int, int);
void cleanup(int*, int);

int main (int argc, char *argv[])
{
	int i, j, length, size, framecount, fifo, lru, index, frame[128]={-1}, future[10]={-1};
	int count, opt, replacement, furthest, furthestcount;
	int *input = malloc(256*sizeof(int));
	char line[256];
	FILE *f;
	f = fopen(argv[1], "r");
	if (f == NULL)
	{
		fprintf(stderr, "Could not open input file \"%s\"\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	while (fgets(line, sizeof(line), f))
	{
		size = 0;
		fifo = 0;
		lru = 0;
		opt = 0;
		index = 0;
		furthest = 0;
		framecount = line[0]-'0';
		length = strlen(line);

		//Loop strips leading framecount and puts integers in array.
		for(i=0; i < length-1; i++)
		{
			if(isdigit(line[i+1]))
			{
				input[size] = line[i+1]-'0';
				size++;
			}
			
		}
		//FIFO Loop
		for(i=0; i < size; i++)
		{
			if(frameexists(frame, input[i], framecount) == -1)
			{
				for(j=0; j < framecount; j++)
				{
						frame[j] = frame[j+1];
				}
				frame[framecount-1] = input[i];
				fifo++;
			}
		}
		printf("FIFO:%d ", fifo);
		cleanup(frame, framecount);

		//LRU Loop
		for(i=0; i < size; i++)
		{
			index = frameexists(frame, input[i], framecount);
			if (index >= 0)
			{
				for(j=0; j < index; j++)
				{
					frame[index-j] = frame[index-j-1];
				}
				frame[0] = input[i];
			}
			else
			{
				for(j=0; j < framecount; j++)
				{
					frame[framecount-j]= frame[framecount-j-1];
				}
				frame[0] = input[i];
				lru++;
			}
		}
		printf("LRU:%d ", lru);
		cleanup(frame, framecount);

		//Optimal Loop
		for(i=0; i < size; i++)
		{
			cleanup(future, 10);
			count = 0;
			furthest = 0;
			furthestcount = 0;
			if(frameexists(frame, input[i], framecount) == -1)
			{
				if(frameexists(frame, -1, framecount)>=0)
				{
					frame[frameexists(frame, -1, framecount)] = input[i];
					opt++;
				}
				else
				{
					for(j=i+1; j<size; j++)
					{
						count++;
						if((frameexists(frame, input[j], framecount) >= 0) && (future[input[j]] < count) && (future[input[j]] < 1))
						{
							future[input[j]] = count;
						}
					}
					for(j=0; j<10; j++)
					{
						if((frameexists(frame, j, framecount) >= 0) && (future[j] == -1))
						{
							furthestcount = 100000;
							furthest = j;
						}
						if(future[j] > furthestcount)
						{
							furthestcount = future[j];
							furthest = j;
						}
					}
					replacement = frameexists(frame, furthest, framecount);
					frame[replacement] = input[i];
					opt++;
				}
			}
		}
		printf("Optimal:%d\n", opt);
		cleanup(frame, framecount);
	}
	free(input);
	return 0;
}

int frameexists(int *frame, int value, int count)
{
	int i;
	for(i=0; i < count; i++)
	{
		if (frame[i] == value)
			return i;
	}
	return -1;
}

void cleanup(int *frame, int framecount)
{
	int i;
	for(i=0; i<framecount+1; i++)
	{
		frame[i]=-1;
	}
}