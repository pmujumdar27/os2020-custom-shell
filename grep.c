#include<stdio.h>
#include<stdlib.h>
#include<regex.h> 
#include<string.h>
#include<errno.h>
#include<signal.h>

#include "globals.h"

FILE *fp;

void input_end_handler(int signum){
    signal(SIGINT, input_end_handler);
    printf("The signal code is: %d\n", signum);
    fclose(fp);
}

void custom_grep(struct parsed_cmd *pc){
    
    char *line = NULL;
    size_t len = 0;
    ssize_t lsz;

    if(pc->argc==1){
        fprintf(stderr, "grep: <searchterm> <filename>\n");
        exit(EXIT_FAILURE);
    }
    else if(pc->argc>=2){
        regex_t regex;
        int rv;
        if(pc->argc==3){
            fp = fopen(pc->argv[2], "r");
            if(fp==NULL){
                fprintf(stderr, "grep: %s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
        }
        if(pc->argc==2){ // if file not given in the arguments, read input from stdin
            fp = stdin;
            if(pc->bgex==1){
                fp = NULL;
            }
        }
        while((lsz=getline(&line, &len, fp))!=-1){ // read lines one by one and check if pattern matches using regex, print line if matches
            signal(SIGINT, input_end_handler);
            rv = regcomp(&regex, pc->argv[1], 0);
            rv = regexec(&regex, line, 0, NULL, 0);
            if(rv==0){
                printf("%s", line);
            }
            else if(rv==REG_NOMATCH){
                ;
            }
            else{
                fprintf(stderr, "grep: %s\n", strerror(errno));
            }
        }
        signal(SIGINT, input_end_handler); //close input on Ctrl+C (still has bugs, shell closes)
        if(pc->argc==3) fclose(fp);
        exit(EXIT_SUCCESS);
    }
}