#include <unistd.h>
#include <stdio.h>

int echo(char **tokens)
{
	int i=1;
	while(tokens[i]!=NULL)
		printf("%s ", tokens[i++]);
	printf("\n");
}