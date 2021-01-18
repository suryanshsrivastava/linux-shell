#include "def.h"
#define HISTORY_COUNT 20

char *cmd;
void term(char* pipetoken);

void welcomeScreen()
{
	printf("\tHey!\n");
}

void sig_handle(int sig)
{
	if(sig == SIGINT)
		fprintf(stderr,"\n");
}
void SIGINT_handler();
void SIGCHLD_handler();

void SIGINT_handler()
{
	signal(SIGINT, SIG_IGN);
	printf("Handled\n");
}
void SIGCHLD_handler()
{
	printf("Process exited!\n");
}

int main(int argc, char *argv[]) 
{
	char* hist[HISTORY_COUNT];

	getcwd(home, sizeof(home));
	welcomeScreen();

	for (int i = 0; i < 1024; ++i){
		joblist[i].bg = 0;
		joblist[i].stat = 0;	
	}
	numproc = 0;

	joblist[0].pid = 0;
	joblist[0].bg = 0;
	joblist[0].stat = 1;
	strcpy(joblist[0].pname,"./shell");
	numproc++;

	int current = 0;
	for (int i = 0; i < HISTORY_COUNT; i++) hist[i] = NULL;

	int i = current;
	while(1)
	{
		finished_bg_procs();
		signal(SIGINT, sig_handle);
		signal(SIGQUIT, sig_handle);
		signal(SIGTSTP, sig_handle);

        // Saving the history to a file
		char wd[1024];
		if(!getcwd(wd, sizeof(wd)))
			perror("cwd error");

		char* cddir1[]={"cd", "~/"};
		change_dir(cddir1, home);
		fptr = fopen("history.txt","a");

		char* cddir2[] = {"cd", wd};
		change_dir(cddir2, home);
		if(fptr == NULL)
		{
			printf("Error!");   
			exit(1);             
		}
		do 
		{
			if (hist[i])
				fprintf(fptr,"%s", hist[i]);
			i = (i + 1) % HISTORY_COUNT;
		} while (i != current);
		fclose(fptr);
		
		prompt(home);
		
		cmd=(char *)malloc(100*sizeof(char));
		size_t nbytes = 100;
		if(getline(&cmd, &nbytes, stdin)==-1)
		{  
			printf("\n");
			fprintf(stderr, "%d\n", errno);
			exit(0);
		}
		
		free(hist[current]);
		hist[current] = strdup(cmd);
		current = (current + 1) % HISTORY_COUNT;

		char *saveptr1, *saveptr2;
		char *str1, *str2 , *token, *pipetoken;
		int j, commands;

		char** tokens;
		tokens=(char**)malloc(100*sizeof(char*));
		for(int i=0 ; i<100 ; i++)
			tokens[i]=(char*)malloc(100*sizeof(char));

		char** pipe_parts;
		pipe_parts = (char**)malloc(200*sizeof(char*));
		for(int i = 0; i < 200 ; i++)
			pipe_parts[i]=(char*)malloc(200*sizeof(char));

		for(int i=0;i<200;i++) pipe_parts[i] = NULL;

		for (j = 0, str1 = cmd; ; j++, str1 = NULL) 
		{
			token = strtok_r(str1, ";\n", &saveptr1);
			tokens[j]=token;
			// printf("token:%i %s\n",j, token );
			if (token == NULL)
				break;

			int fd[2];
			int fdc = 0;
			for (commands=0, str2 = token; ;commands++, str2 = NULL) 
			{
				pipetoken = strtok_r(str2, "|", &saveptr2);
				pipe_parts[commands] = pipetoken;
				// printf("pipetoken:%i %s\n",commands, pipetoken );
				if (pipetoken == NULL)
					break;
			}
			int itr =0;
			if(pipe_parts[itr+1] == NULL)
			{
				term(pipe_parts[itr]);
				i++;
				continue;
			}
			while(pipe_parts[itr]!=NULL)
			{
				pipe(fd);
				int fk = fork(); 
				if(fk==0)
				{
					// printf("child i: %i\n", itr);
					dup2(fdc,0);
						// printf("%s\n", pipe_parts[itr+1]);
					
					if(pipe_parts[itr+1]){
						if(dup2(fd[1],1)<0)
							perror("input duping error");
					}

					close(fd[0]);
					term(pipe_parts[itr]);	
					exit(0);
				}
				else
				{
					wait(NULL);
					// printf("parent i: %i\n", itr);
					itr++;
					fdc = dup(fd[0]);
					close(fd[1]);
				}
			}
		}
		
	}
	return 0;
}

void term(char* pipetoken)
{
	int arguments;
	char* str3; char* subtoken;
	char** command;

	command = (char**)malloc(200*sizeof(char*));
	for(int i = 0; i < 200 ; i++)
		command[i]=(char*)malloc(200*sizeof(char));
	
	for (arguments=0, str3 = pipetoken; ;arguments++, str3 = NULL) 
	{
		subtoken = strtok(str3, " \t");
		command[arguments] = subtoken;
					// printf("subtoken:%i %s\n",arguments, subtoken );
		if (subtoken == NULL)
			break;
	}

	int termout = dup(1);
	int termin = dup(0);
	ioredir(command, arguments);

	if (arguments>1)
		if(!strcmp(command[arguments-2], ">"))
			command[arguments-2]=NULL;
		else if (!strcmp(command[arguments-2], ">>"))
		{
			command[arguments-2]=NULL;
		}
		else if (!strcmp(command[arguments-2], "<"))
		{
			command[arguments-2]=NULL;
		}

	int background = 0;
	if(!strcmp(command[arguments-1], "&"))
	{
		command[arguments-1] = '\0';	
		background = 1;
	}

	if (background)
	{
		printf("This command when finished, should print result to stdout.\n");
		printf("%s %s\n", command[0], command[1]);
		execute(command, background);	
	}
	else
	{
		if(strcmp(command[0], "cd")==0)
			change_dir(command, home);
		else if(strcmp(command[0], "pwd")==0)
			pwd(command, home);
		else if(strcmp(command[0], "echo")==0)
			echo(command);
		else if(strcmp(command[0], "ls")==0)
			ls(command);
		else if(strcmp(command[0], "pinfo")==0)
			pinfo(command, arguments, home);
		else if (strcmp(cmd, "history") == 0)
			history(command, arguments, home);
		else if (strcmp(cmd, "setenv") == 0)
			set_env(command, arguments);
		else if (strcmp(cmd, "unsetenv") == 0)
			unset_env(command, arguments);
		else if (strcmp(cmd, "kjobs") == 0)
			kjobs(command, arguments);
		else if (strcmp(cmd, "overkill")==0)
			overkill();
		else if (strcmp(cmd, "jobs")==0)
		{
			int i;
			printf("%d\n", numproc);
			for(i=0;i<numproc;i++)
				printf("[%d] %d %s [%d]\n", (i+1),joblist[i].stat ,joblist[i].pname,joblist[i].pid);
		}
		else if (strcmp(cmd,"bg")==0)
		{
			if (arguments<2)
				printf("enter bg [JOB_NO]\n");
			else if (joblist[atoi(command[1])].stat==0)
			{
				joblist[atoi(command[1])].stat=1;
				kill(joblist[atoi(command[1])].pid,SIGCONT);
			}
		}
		else if (strcmp(cmd,"fg")==0)
		{
			current_pid = joblist[atoi(command[1])].pid;
			if (arguments<2)
				printf("enter bg [JOB_NO]\n");
			else if (joblist[atoi(command[1])].stat==0)
			{
				joblist[atoi(command[1])].stat=1;
				kill(joblist[atoi(command[1])].pid,SIGCONT);
			}
			int stat;
			waitpid(joblist[atoi(command[1])].pid, &stat, WUNTRACED);
		}
		else if(!strcmp(command[0], "quit"))
		{
			printf("See ya!\n");
			exit(0);
		}
		else
			execute(command, background);

		if(termin != 0)
		{
			dup2(termin,0);
			close(termin);
		}
		if(termout != 1)
		{
			dup2(termout,1);
			close(termout);
		}
	}
}