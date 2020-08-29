#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>

#include "globals.h"

void custom_mkdir(struct parsed_cmd *pc){
    if(pc->argc < 2){
        fprintf(stderr, "mkdir: Please specify the path of the directory you want to create\n");
        exit(EXIT_FAILURE);
    }
    if(mkdir(pc->argv[1], 0777)==-1){
        fprintf(stderr, "mkdir: Couldn't create directory\n");
        exit(EXIT_FAILURE);
    }
    printf("Directory created\n");
    exit(EXIT_SUCCESS);
}