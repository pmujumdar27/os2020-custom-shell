#include<stdio.h>
#include<stdlib.h>

#include "globals.h"

void custom_cat(struct parsed_cmd *pc){
    FILE *fptr;
    fptr = fopen(pc->argv[1], "r");
    if(fptr==NULL){
        fprintf(stderr, "cat: Error in opening the file/couldn't locate the file\n");
        exit(EXIT_FAILURE);
    }
    char c = fgetc(fptr);
    while(c!=EOF){
        printf("%c", c);
        c = fgetc(fptr);
    }
    fclose(fptr);
    exit(EXIT_SUCCESS);
}