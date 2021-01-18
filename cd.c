#include <stdio.h>
#include <unistd.h>
#include <string.h>

void change_dir(char **cd_path, char *home)
{
	char *input_path;
	if (cd_path[1])
		input_path = cd_path[1];
	else
		input_path = "~";
	char path[256];
	if(input_path[0]=='~')
	{
		int k=0;
		for(int i=0 ; i<strlen(home); i++)
			path[k++]=home[i];
		for(int i=1; i<strlen(input_path); i++)
			path[k++]=input_path[i];
		path[k]='\0';
	}
	else
		strcpy(path, input_path);

	if(chdir(path) < 0)
	{
		perror("chdir error");
		return;
	}
}