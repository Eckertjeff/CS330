#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void formatInput(int*, char*, int*, int*);
int FIFO(int*, int, int*, int);
int LRU(int*, int, int*, int);
int Optimal(int*, int, int*, int, int*);
int frameexists(int*, int, int);
void cleanup(int*, int);

int main (int argc, char *argv[])
{
	int length, size, framecount, fifo, lru, opt;
	int *input = malloc(256*sizeof(int));
	int *frame = malloc(64*sizeof(int));
	int *future = malloc(256*sizeof(int));
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
		formatInput(input, line, &size, &framecount);
		fifo = FIFO(input, size, frame, framecount);
		lru = LRU(input, size, frame, framecount);
		opt = Optimal(input, size, frame, framecount, future);
		printf("FIFO:%d LRU:%d Optimal:%d\n", fifo, lru, opt);
	}
	free(input);
	free(frame);
	free(future);
	return EXIT_SUCCESS;
}

/*
Takes the first character in the line read in from the file and uses it to 
determine the number of available frames and stores it in framecount.
Next the function checks to see if the remaining line of input is a digit, if
it is a digit then it is stored in the input array.  The input array's size is
kept track of for use in the FIFO, LRU, and Optimal functions.
*/
void formatInput(int *input, char *line, int *size, int *framecount)
{
	int i, length;
	*size = 0;
	length = strlen(line);
	*framecount = line[0]-'0';
	for(i=0; i < length-1; i++)
	{
		if(isdigit(line[i+1]))
		{
			input[*size] = line[i+1]-'0';
			*size = *size + 1;
		}	
	}
}

/*
Traverses the input, checking if the page requested exists in the "memory frame".
If the page requested from the input is not in the memory frame, the page
is added to the memory frame using a first-in first-out page replacement policy.
The memory frame is then cleaned up to prepare for the next simulation and the
number of page faults is returned.
*/
int FIFO(int *input, int size, int *frame, int framecount)
{
	int i, j, fifo=0;
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
	cleanup(frame, framecount);
	return fifo;
}

/*
Traverses the input, checking if the page requested is in the memory frame.
If the page requested from the input is not in the memory frame, the page
is added to the memory frame using a least recently used page replacement policy.
If the page requested from the input is found in the memory frame, the memory frame
is sorted to account for the page being used in a least recently used page replacement
policy and the process continues.
The memory frame is then cleaned up to prepare for the next simulation and the
number of page faults is returned.
*/
int LRU(int *input, int size, int *frame, int framecount)
{
	int i, j, lru=0, index;
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
	cleanup(frame, framecount);
	return lru;
}

/*
Traverses the input, checking if the page requested is in the memory frame.
If the page is not found, it follows the Optimal page replacement policy by
checking to see if there are any unused frames. If so the function adds the page
there and increments the page fault counter. Otherwise a new array is assembled to
see how far in the future the current pages in memory will be requested again. The
page in memory that will be requested at the furthest point in time is then replaced
with the currently requested page, and the page fault counter is incremented. The
memory frame is then cleaned up and the number of page faults is returned.
*/
int Optimal(int *input, int size, int *frame, int framecount, int *future)
{
	int i, j, count, furthest, furthestcount, replacement, opt=0;
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
	cleanup(frame, framecount);
	return opt;
}

//Searches the frame, if the value exists, return the position.
//Otherwise return -1. 
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

//Resets all the previously used positions in the frame array to -1.
void cleanup(int *frame, int framecount)
{
	int i;
	for(i=0; i<framecount+1; i++)
	{
		frame[i]=-1;
	}
}