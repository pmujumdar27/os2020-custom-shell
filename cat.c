#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>

#include "globals.h"

void custom_cat(struct parsed_cmd *pc){
    FILE *fptr;
    fptr = fopen(pc->argv[1], "r"); //opening the file
    if(fptr==NULL){ //error handling
        fprintf(stderr, "cat: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    char c = fgetc(fptr);

    // printing all the characters in the file till we reach EOF
    while(c!=EOF){
        printf("%c", c);
        c = fgetc(fptr);
    }
    fclose(fptr);
    exit(EXIT_SUCCESS);
}