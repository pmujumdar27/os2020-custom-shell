#include<stdlib.h>
#include<stdio.h>

// This struct stores the number of input arguments (excluding '&'), the arguments and if background execution is requested
struct parsed_cmd{
    int argc; //argument count
    char **argv; //arguments
    int bgex; //1 when '&' at the end of the command
};

// function to dynamically create the parsed_cmd structure
struct parsed_cmd *create_cmd(int argc, char **argv){
    struct parsed_cmd *pc = (struct parsed_cmd*)malloc(sizeof(struct parsed_cmd));
    pc->argc = argc;
    pc->argv = argv;
    pc->bgex = 0;
    return pc;
}