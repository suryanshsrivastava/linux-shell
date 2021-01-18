#include "def.h"

void pinfo(char* argv[], int argc, char* home)
{
    if(argc > 2)
    {
        printf("pinfo: Usage: pinfo <pid>\n");
        return;
    }
    int pid;
    char* pid_string = (char*) malloc(100*sizeof(char*));
    if(argc == 1)
        strcpy(pid_string, "self");
    else
        pid_string = argv[1];

    char* statpath = (char *)malloc(100*sizeof(char));
    strcpy(statpath, "/proc/");
    strcat(statpath, pid_string);strcat(statpath, "/stat");
    printf("%s\n", pid_string);

    struct stat filestat;
    if(stat(statpath, &filestat)<0)
    {
        perror("PID does not exist");
        return;
    }

    char* file_buffer = (char*) malloc(100*sizeof(char*));

    FILE *statfile;
    statfile = fopen(statpath, "r");
    fread(file_buffer, sizeof(file_buffer), 1024, statfile);
   	char* split;
    for (int i = 0; ;i++, file_buffer = NULL) 
	{
		split = strtok(file_buffer, " \n\t");
		if(i==0) printf("pid -- %s\n", split);
        else if(i==2) printf("Process Status -- %s\n", split);
        else if(i==22) printf("memory -- %s\t{​Virtual Memory}\n", split);
		
		if (split == NULL)
			break;
	}

    char* exec_path = (char*) malloc(4096);
    strcpy(exec_path, "/proc/");
    strcat(exec_path, pid_string);strcat(exec_path, "/exe");

    char* read_buff = (char*) malloc(4096);
    int exec_check = readlink(exec_path, read_buff, 4095);
    read_buff[4095] = '\0';
    if(exec_check<0)
    {
        perror("exec file can't be accessed");
        return;
    }

	read_buff = path(home, read_buff);

    printf("Executable Path -- %s\n", read_buff);
    return;
}
