#include "def.h"

void history(char** command, int argc, char* home)
{
	int line_num = 10;
	if (argc==2)
		line_num = command[1][0]-'0';
  	
  	char wd[1024];
  	if(!getcwd(wd, sizeof(wd)))
		perror("cwd error");
  	
  	char* cddir1[]={"cd", "~/"};
  	change_dir(cddir1, home);
  	fptr = fopen("history.txt","r");
  	
  	char* cddir2[] = {"cd", wd};
  	change_dir(cddir2, home);

   	if(fptr == NULL)
   	{
    	printf("Error!");   
      	exit(1);             
   	}
   	char line[256];
   	
    while (fgets(line, sizeof(line), fptr) && line_num--) {
        printf("%s", line); 
    }
    fclose(fptr);

	return;
}