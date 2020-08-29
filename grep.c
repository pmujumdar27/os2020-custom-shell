#include<stdio.h>
#include<stdlib.h>
#include<regex.h> 

#include "globals.h"

void custom_grep(struct parsed_cmd *pc){
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t lsz;

    if(pc->argc==1){
        fprintf(stderr, "grep: <searchterm> <filename>\n");
        exit(EXIT_FAILURE);
    }
    // else if(pc->argc==2){
    //     // Take input from stdin
    // }
    else if(pc->argc>=2){
        regex_t regex;
        int rv;
        if(pc->argc==3){
            fp = fopen(pc->argv[2], "r");
            if(fp==NULL){
                fprintf(stderr, "grep: cannot open file\n");
                exit(EXIT_FAILURE);
            }
        }
        if(pc->argc==2){
            fp = stdin;
        }
        while((lsz=getline(&line, &len, fp))!=-1){
            rv = regcomp(&regex, pc->argv[1], 0);
            rv = regexec(&regex, line, 0, NULL, 0);
            if(rv==0){
                printf("%s", line);
            }
            else if(rv==REG_NOMATCH){
                ;
            }
            else{
                fprintf(stderr, "Error in matching regex\n");
            }
        }
        if(pc->argc==3) fclose(fp);
        exit(EXIT_SUCCESS);
    }
}