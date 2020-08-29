#include<stdlib.h>
#include<stdio.h>
#include<dirent.h>

#include "globals.h"

void ls_helper(char *path){
    DIR *dp;
    if((dp=opendir(path))==NULL){
        fprintf(stderr, "custom_ls: cannot open directory %s\n", path);
        exit(EXIT_FAILURE);
    }
    printf("%s:\n", path);
    struct dirent *d;
    while((d=readdir(dp))!=NULL){
        printf("%s\t", d->d_name);
    }
    printf("\n");
}

void custom_ls(struct parsed_cmd *pc){
    char *path = ".";
    if(pc->argc > 1){
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