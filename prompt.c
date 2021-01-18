#include <stdio.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>

#define RED   "\x1B[31m"
#define YEL   "\x1B[33m"
#define RESET "\x1B[0m"

char* path(char* home, char* cwd)
{
  char* curr_dir = (char*) malloc (1024);
  int i=0;
  // if the cwd unmatches before traversing the length of home, length of path is less than the home path, path is printed as it is
  for(; i<strlen(home); i++)
  {
    if(cwd[i] != home[i])
      return cwd;
  }
  curr_dir[0] = '~';
  // if the cwd matches the length of home, we compare the cwd and home. if they don't match append the remainder of cwd to the final path
  if(strcmp(home,cwd))
  {
    for(;i<strlen(cwd);i++)
      curr_dir[i-strlen(home)+1] = cwd[i];
  }
  curr_dir[strlen(cwd) - strlen(home) + 1] = '\0';
  return curr_dir;
}

void prompt(char* home){
    struct passwd *username;
  	username = getpwuid(getuid());
  	
  	char hostbuffer[256]; 
  	int hostname;
  	hostname = gethostname(hostbuffer, sizeof(hostbuffer)); 
  	
  	char cwd[1024]; 
    getcwd(cwd, sizeof(cwd)); 

  	if(username == NULL)
  	{
  		perror("username error");
  		exit(1);
  	}
  	else if (hostname == -1)
  	{
  		perror("hostname error"); 
        exit(1); 
  	}
  	else
  	{
      char* dir = (char*) malloc (1024);
    	dir = path(home, cwd);
      fprintf(stdout, "<" YEL "%s" RESET RED "@%s" RESET ":%s>", username->pw_name, hostbuffer, dir);
  	}
}
