#include "def.h"

void ioredir(char *argv[], int argc)
{
	int i,fdin=0,fdout=0,fdappend=0;
	char* input_file;
	input_file=(char*)malloc(100*sizeof(char));

	for (int i = 0; i < argc; ++i)
	{
		if(!strcmp(argv[i],"<"))
		{
			input_file = argv[i+1];
			// printf("%s\n", input_file);
			if((fdin = open(input_file, O_RDONLY))<0) perror("Error opening input file");
			if(dup2(fdin,STDIN_FILENO) < 0) perror("input duping error");
			close(fdin);
		}
		else if(argv[i][0]=='>')
		{
			if(argv[i][1]=='>')
				fdout=open(argv[i+1],O_CREAT | O_WRONLY |O_APPEND, 0644);
			else
				fdout=open(argv[i+1],O_CREAT | O_WRONLY | O_TRUNC, 0644);
			if(fdout<0) perror("Error opening output file");
			if(dup2(fdout,STDOUT_FILENO)<0) perror("output duping error");
			close(fdout);
		}
	}
	return;
}
