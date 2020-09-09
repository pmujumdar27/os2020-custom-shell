#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>

#include "globals.h"

// function to remove a single file
void remove_file(char *path){
    // printf("filepath to remove: %s\n", path); //debugging 
    if(remove(path)<0){
        fprintf(stderr, "rm: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

// function checking syntax for rm and executing appropriate instruction
void custom_rm(struct parsed_cmd *pc){
    if(pc->argc==1 || pc->argc > 3){
        fprintf(stderr, "rm <filepath>\n");
        exit(EXIT_FAILURE);
    }
    else if(pc->argc==2){
        if(strcmp(pc->argv[1], "-r")==0){
            fprintf(stderr, "rm -r <directory>\n");
            exit(EXIT_FAILURE);
        }
        remove_file(pc->argv[1]);
    }
    else if(pc->argc==3){
        if(strcmp(pc->argv[1], "-r")==0){ //execution of rm -r command
            if(rmdir(pc->argv[2])<0){
                fprintf(stderr, "rm: %s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
        }
        else{
            fprintf(stderr, "rm -r <directory>\n");
            exit(EXIT_FAILURE);
        }
    }
    exit(EXIT_SUCCESS);
}