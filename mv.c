#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<fcntl.h>

#include "globals.h"

void custom_mv(struct parsed_cmd *pc){
    if(pc->argc < 3){
        fprintf(stderr, "mv: <source_path> <destination_path>\n");
        exit(EXIT_FAILURE);
    }
    else{
        if(rename(pc->argv[1], pc->argv[2])==-1){ //moves the file from source to the destination
            fprintf(stderr, "mv: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }
}