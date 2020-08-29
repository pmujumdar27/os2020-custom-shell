#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>

#include "globals.h"

void custom_pwd(){
    char cwd[256];
    // char *cwd = (char*)malloc(256*sizeof(char));
    if(cwd==NULL){
        fprintf(stderr, "pwd: Error in finding pwd");
        exit(EXIT_FAILURE);
    }
    printf("%s\n", getcwd(cwd, sizeof(cwd)));
    // free(cwd);
    exit(EXIT_SUCCESS);
}