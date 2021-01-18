#include "def.h"

void set_env(char* argv1[], int argc1)
{
    char* new_val = (char*)malloc(4096);
    if (argc1 == 3 || argc1 == 2)
    {
        if (argc1 == 3)
            new_val = argv1[2];
        else
            strcpy(new_val,"");
        
        if(setenv(argv1[1],new_val,1))
            perror("error");
    }
    else 
        printf("setenv: Usage: setenv VAR [value]\n");
    return;
}

void unset_env(char* argv1[], int argc1)
{
    if(argc1 == 2)
        if(unsetenv(argv1[1]))
            printf("variable does not exist\n");
    else
        printf("unsetenv: Usage: unsetenv VAR\n");
    return;
}