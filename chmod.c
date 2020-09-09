#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/stat.h>
#include<fcntl.h>

#include "globals.h"

void custom_chmod(struct parsed_cmd *pc){
    if(pc->argc <3 || pc->argc > 3){
        fprintf(stderr, "chmod: <mode> <pathname>\n");
        exit(EXIT_FAILURE);
    }
    else{
        mode_t mode = strtol(pc->argv[1], 0, 8); // converts octal string
        // printf("mode entered: %d\n", mode); //debugging
        if(chmod(pc->argv[2], mode)==-1){
            fprintf(stderr, "chmod: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }
}