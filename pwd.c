#include <unistd.h>
#include <stdio.h>

void pwd()
{
  char path[1024];
  if(!getcwd(path, sizeof(path)))
		perror("cwd error");
  printf("%s\n", path);
}