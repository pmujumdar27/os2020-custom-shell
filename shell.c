#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>

// including headers for commands and global structs
#include "globals.h"
#include "pwd.h"
#include "ls.h"
#include "mkdir.h"
#include "cat.h"
#include "grep.h"
#include "mv.h"
#include "cp.h"
#include "chmod.h"
#include "rm.h"

#define MAX_COMM_SIZE 2000
#define ARGSIZE 100
#define CUSTOM_COUNT 9

char CWD[256];

char *SHELL_COMMANDS[3] = {"cd", "help", "exit"};
char *custom_comms[CUSTOM_COUNT] = {"pwd", "ls", "mkdir", "cat", "grep", "mv", "cp", "chmod", "rm"};

int EXIT_SHELL = 0;

char *get_input(){
    int cmd_size = MAX_COMM_SIZE;
    char *cmd = (char*)malloc(cmd_size*sizeof(char));
    int ptr = 0;
    char c;

    if(cmd==NULL){
        // character array for input command couldn't be allocated so ERROR
        fprintf(stderr, "%s \n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // take character input till we get EOF or newline
    while(1){
        c = getchar();

        if(ptr>=cmd_size){
            // command is too big so reallocate memory

            cmd_size += MAX_COMM_SIZE;
            cmd = realloc(cmd, cmd_size);
            if(cmd==NULL){
                fprintf(stderr, "input_mem_alloc: %s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
        }

        if(c==EOF || c=='\n'){
            cmd[ptr] = '\0';
            return cmd;
        }
        else{
            cmd[ptr] = c;
        }
        ptr++;
    }
}

// function for breaking the command into argument count and argument strings and checking if background execution is expected
struct parsed_cmd* parse_args(char *command){
    int argsize = ARGSIZE;
    char ** args = malloc(argsize*sizeof(char*));
    char *arg;

    int ptr = 0;

    if(args==NULL){
        // memory allocation error
        fprintf(stderr, "%s \n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    int bg = 0;

    arg = strtok(command, " \t\r\a\n");
    while(arg!=NULL){
        // printf("%s ", arg);
        if(strcmp(arg, "&")==0){
            bg=1;
        }
        else{
            args[ptr] = arg;
            ptr++;
        }

        if(ptr>=argsize){
            argsize += ARGSIZE;
            args = realloc(args, argsize*sizeof(char*));
            if(args==NULL){
                // memory reallocation error
                fprintf(stderr, "%s \n", strerror(errno));
                exit(EXIT_FAILURE);
            }
        }

        arg = strtok(NULL, " \t\r\a\n");
    }
    args[ptr] = NULL;
    printf("\n");
    struct parsed_cmd *pc = create_cmd(ptr, args);
    pc->bgex = bg;
    return pc;
}

// function for executing the parsed command
int execute_command(struct parsed_cmd* pc, int builtin_flag){
    char **args = pc->argv;
    if(builtin_flag<0){
        int rc = fork();
        if(rc<0){
            // error in fork
            fprintf(stderr, "%s \n",strerror(errno));
            exit(EXIT_FAILURE);
        }
        else if(rc==0){
            // child process
            // flag to tell if it's a custom command
            
            if(pc->bgex==1){
                printf("PID: %d\n", (int)getpid());
            }

            int custom_comm = -1;

            for(int i=0; i<CUSTOM_COUNT; i++){
                if(strcmp(args[0], custom_comms[i])==0){
                    custom_comm = i;
                    break;
                }
            }

            if(custom_comm>=0){
                if(custom_comm==0){
                    custom_pwd();
                }
                else if(custom_comm==1){
                    custom_ls(pc);
                }
                else if(custom_comm==2){
                    custom_mkdir(pc);
                }
                else if(custom_comm==3){
                    custom_cat(pc);
                }
                else if(custom_comm==4){
                    custom_grep(pc);
                }
                else if(custom_comm==5){
                    custom_mv(pc);
                }
                else if(custom_comm==6){
                    custom_cp(pc);
                }
                else if(custom_comm==7){
                    custom_chmod(pc);
                }
                else if(custom_comm==8){
                    custom_rm(pc);
                }
            }
            else{
                // Use inbuilt binary from the OS to execute the command
                printf("Using inbuilt binary\n"); //Comment this later
                if(execvp(args[0], args)==-1){
                    // Error in executing command
                    fprintf(stderr, "%s \n", strerror(errno));
                    exit(EXIT_FAILURE);
                }
                else{
                    printf("\n");
                    wait(NULL);
                    return 0;
                }
            }
        }
    }
    else if(builtin_flag==0){
        int cdr_success = chdir(args[1]);
        if(cdr_success==-1){
            fprintf(stderr, "cd: %s \n", strerror(errno));
        }
    }
    else if(builtin_flag==1){
        printf("This custom shell is made by Pushkar Mujumdar\nRefer to the README to know more.\nThank you for using this shell.\n");
    }
    else if(builtin_flag==2){
        printf("Exiting ...\n");
        EXIT_SHELL = 1;
        return 1;
    }
    return 0;
}

void shell_loop(){
    char *command;
    struct parsed_cmd* pc;
    char *username = getenv("USER");
    while(EXIT_SHELL==0){
        char ESC=27;
        printf("%c[4m",ESC); //underlined text start
        printf("\n%s", username);
        printf("%c[0m",ESC); //underlined text end
        printf("%c[1m",ESC);  //bold text start
        printf("@ %s\n> $ ", getcwd(CWD, sizeof(CWD)));
        printf("%c[0m",ESC); //bold text end
        // taking input
        command = get_input();

        int builtin_flag = -1;

        // parsing input
        pc = parse_args(command);

        // checking if it's one of the commands that does not need a fork 
        for(int i=0; i<3; i++){
            if(strcmp(pc->argv[0], SHELL_COMMANDS[i])==0){
                builtin_flag = i;
            }
        }

        // executing the entered command
        int EXIT_NOW = execute_command(pc, builtin_flag);
        if(pc->bgex==0){
            wait(NULL);
        }
        
        free(command);
        free(pc);
        if(EXIT_NOW==1){
            return;
        }
    }
}

int main(int argc, char *argv[]){
    system("clear");
    shell_loop();
    return EXIT_SUCCESS;
}