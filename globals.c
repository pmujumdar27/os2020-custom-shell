#include<stdlib.h>
#include<stdio.h>

struct parsed_cmd{
    int argc;
    char **argv;
};

struct parsed_cmd *create_cmd(int argc, char **argv){
    struct parsed_cmd *pc = (struct parsed_cmd*)malloc(sizeof(struct parsed_cmd));
    pc->argc = argc;
    pc->argv = argv;
    return pc;
}