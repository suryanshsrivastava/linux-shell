#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <grp.h>
#include <time.h>
#include <unistd.h>
#include <pwd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define RED   "\x1B[31m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define RESET "\x1B[0m"

char home[1024];
typedef struct 
{
	pid_t pid;
	char pname[1024];
	int stat;
	int bg;
}process;
pid_t shellpid;
pid_t current_pid;
int numproc;
process joblist[1024];

FILE *fptr;

char* path(char* home, char* cwd);
void prompt(char *home);

void change_dir(char **cd_path, char *home);

void pwd();

void echo(char **tokens);

void ls(char** cmd);
void detailedls(struct dirent *entry);

void finished_bg_procs();
void execute(char **cmd, int bg);

void pinfo(char* argv[], int argc, char* home);

void history(char** command, int argc, char* home);

void set_env(char* argv1[], int argc1);
void unset_env(char* argv1[], int argc1);

void ioredir(char *argv[], int argc);

void kjobs(char* argv1[], int argc1);
void overkill();

void ctrlC(int signo);
void ctrlZ(int signo);