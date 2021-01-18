#include "def.h"

void ls(char** cmd)
{
	char* path;
	path = (char*)malloc(100*sizeof(char));
	struct dirent *entry;
	DIR *dp;

	if(cmd[1])
		if (cmd[1][0]=='-')
		{
			if(cmd[2])
				path = cmd[2];
			else
				path = ".";

			dp = opendir(path);
			if (dp == NULL) 
			{
				perror("opendir error");
				return;
			}

			if (strstr(cmd[1],"l"))
			{
				if(!strstr(cmd[1],"a"))
				{
					while((entry = readdir(dp)))
						if(entry->d_name[0]!=46) detailedls(entry);
						else continue;
					closedir(dp);
					return;
				}
				else
				{
					while((entry = readdir(dp)))
						detailedls(entry);
					closedir(dp);
					return;
				}
			}
			else if (strstr(cmd[1],"a"))
			{
				while((entry = readdir(dp)))
					printf(" %s\n",entry->d_name);
				closedir(dp);
				return;
			}	
		}
		else
			path = cmd[1];
	else
		path = ".";
		
	dp = opendir(path);
	if (dp == NULL) 
	{
		perror("opendir");
		return;
	}

	while((entry = readdir(dp)))
		if(entry->d_name[0]!=46) printf(" %s\n",entry->d_name);
		else continue;
	closedir(dp);
	return;
}

void detailedls(struct dirent *entry)
{
	struct group *grp;
	struct passwd *pwd;

	struct stat fileStat;
	stat(entry->d_name, &fileStat);

	printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
	printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
	printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
	printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
	printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
	printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
	printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
	printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
	printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
	printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");

	printf(" %d",(int)fileStat.st_nlink);

	pwd = getpwuid(geteuid());
	printf(" %s", pwd->pw_name);

	grp = getgrgid((fileStat.st_gid));
	printf(" %s", grp->gr_name);

	printf(" %d\t",(int)fileStat.st_size);

	char* tim = ctime(&fileStat.st_mtime);
	int i;
	for(i=4;i<strlen(tim)-9;i++)
		printf("%c",tim[i]);

	printf(" %d",fileStat.st_uid);

	if (S_ISDIR(fileStat.st_mode))
		printf( BLU " %s\n" RESET,entry->d_name);
	else
		printf(" %s\n",entry->d_name);
}