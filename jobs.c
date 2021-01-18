#include "def.h"


void overkill()
{
	int i;
	for(i=0; i<numproc; i++)
	{
		if (joblist[i].pid == 0)
			continue;
		if (joblist[i].bg == 1)
		{
			printf("%i\n", joblist[i].pid);
			printf("killing process id %d\n",joblist[i].pid);
			kill(joblist[i].pid,SIGKILL);
			joblist[i].pid=-1;
		}
	}
}

void kjobs(char* argv1[], int argc1)
{
	if (argc1 != 3)
		printf("Usage: kjob <jobNumber> <signalNumber>\n");
	else
	{	
	for (int i = 0; i < numproc; ++i)
	{
			kill(joblist[atoi(argv1[1])].pid,atoi(argv1[2]));
			joblist[atoi(argv1[1])].stat = 0;
	}
}
}