#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>

#include "globals.h"

void custom_pwd(){
    char cwd[256];
    if(cwd==NULL){
        fprintf(stderr, "pwd: Error in finding pwd");
        exit(EXIT_FAILURE);
    }
    printf("%s\n", getcwd(cwd, sizeof(cwd))); //prints present working directory
    exit(EXIT_SUCCESS);
}