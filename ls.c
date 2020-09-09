#include<stdlib.h>
#include<stdio.h>
#include<dirent.h>
#include<string.h>
#include<errno.h>

#include "globals.h"

// reads the contents of directory with the given path
void ls_helper(char *path){
    DIR *dp;
    if((dp=opendir(path))==NULL){
        fprintf(stderr, "ls: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    printf("%s:\n", path);
    struct dirent *d;
    while((d=readdir(dp))!=NULL){
        printf("%s\t", d->d_name); //read directory contents and print the names
    }
    printf("\n");
}

void custom_ls(struct parsed_cmd *pc){
    char *path = ".";
    if(pc->argc > 1){ // if multiple arguments, print contents of them one by one
        for(int i=1; i<pc->argc; i++){
            path = pc->argv[i];
            if(i>1){
                printf("\n");
            }
            ls_helper(path);
        }
    }
    else{
        ls_helper(path);
    }
    exit(EXIT_SUCCESS);
}