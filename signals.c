#include "def.h"

// void HandleSignal(int sig, siginfo_t *si, void *context)
// {
// 	int status, pid,i,pos;
// 	switch(sig)	
// 	{
// 		case SIGINT:
// 		fprintf(stderr, "\nControl-C was pressed\n");

// 		break;
// 		case SIGCHLD:
// 		pid = waitpid(-1, NULL, WNOHANG);
// 		if(pid>0)
// 		{
// 			kill(pid , SIGTERM);
// 			if(arr[pid][0]!=0)
// 				fprintf(stderr, "\n%s with pid %d exited!\n" ,arr[pid], pid);
// 			arr[pid][0]=0;
// 			for(i=0;bgjoblist[i].pid!=-1;i++)
// 				if(bgjoblist[i].pid == pid)
// 					pos = i;
// 			for(i=pos;bgjoblist[i+1].pid!=-1;i++)
// 				bgjoblist[i]=bgjoblist[i+1];
// 			bgjoblist[i].pid = -1;
// 		}
// 		break;
// 	 }
// }

void ctrlC(int signo)
{
	if(signo == SIGINT)
		if(getpid() != shellpid)
			kill(getpid(),9);
		else
			printf("\n");
}

void ctrlZ(int signo)
{
	if(signo == SIGTSTP);
}