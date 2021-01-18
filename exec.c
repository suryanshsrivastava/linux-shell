#include "def.h"

void handle_ctrlz(int sig)
{
  if (current_pid)
  {
    printf("Stopping and Sending to bg\n");
    kill(current_pid,SIGTSTP);
    strcpy(joblist[numproc].pname, "current");
    numproc++;
    printf("%d\n", current_pid);
      joblist[numproc].pid=current_pid;
      joblist[numproc].stat = 0;
      current_pid = 0;
      
  }
}
void handle_ctrlc(int sig)
{
  if (current_pid)
    kill(current_pid,SIGINT);
}

void finished_bg_procs()
{
  process active_proc[1024];  
    int pid, status;
    int j=0;
    for (int i = 0; i<numproc; i++)
    {	
    	if(joblist[i].bg && joblist[i].stat)
    	{
    		int wait = waitpid(joblist[i].pid, &status, WNOHANG);
            if(wait>0)
            {     
              if(WIFEXITED(status))
          	     printf("%s with pid %d exited normally\n", joblist[i].pname, joblist[i].pid);
        		  else if(WIFSIGNALED(status))
          			printf("%s with pid %d was interrupted\n", joblist[i].pname, joblist[i].pid);
            }
            else
            {
                active_proc[j] = joblist[i];
                j++;
            }
            for (int i = 0; i < j; ++i)
          joblist[i]=active_proc[i];
          numproc = j;
        }
    }
}

void execute(char **cmd, int bg)
{
  pid_t w;
	int pid = fork();
  // printf("pid = %i\n",pid );
	if(pid == -1)
  {
		perror("Fork Error");
    return;
  }

  if(pid == 0)
  {
    // printf("works\n");
    if (execvp(cmd[0], cmd))
    {
        printf( "%s command unrecognised\n", cmd[0]);
        exit(-1);
    }
  }
  else
  {
    // printf("bg=%d, pid=%d\n", bg, pid);
  		if(bg == 1)
  		{
  	  		fprintf(stdout, "%d started in background\n", pid);
  	  		usleep(70);
            joblist[numproc].pid = pid;
				    strcpy(joblist[numproc].pname, cmd[0]);
            joblist[numproc].stat= 1;
            joblist[numproc].bg = 1;

            numproc++;
  		}
  		else if(bg == 0)
      {
        // printf("waiting\n");
        joblist[numproc].pid = pid;
            strcpy(joblist[numproc].pname, cmd[0]);
            joblist[numproc].stat=1;
            joblist[numproc].bg = 0;
            numproc++;
            current_pid = pid;
            signal(SIGINT, handle_ctrlc);
            signal(SIGTSTP, handle_ctrlz);
            
		    w = waitpid(pid, NULL, WUNTRACED | WCONTINUED);
        current_pid  = 0;
        // printf("wait over\n");
      }
  }
}