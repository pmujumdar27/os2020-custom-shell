#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<dirent.h>

#define MAX_COMM_SIZE 2000
#define ARGSIZE 100
#define CUSTOM_COUNT 2

char CWD[256];

char *SHELL_COMMANDS[3] = {"cd", "help", "exit"};
char *custom_comms[CUSTOM_COUNT] = {"pwd", "ls"};

int EXIT_SHELL = 0;

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

void custom_pwd();
void custom_ls(struct parsed_cmd *pc);

char *get_input(){
    int cmd_size = MAX_COMM_SIZE;
    char *cmd = (char*)malloc(cmd_size*sizeof(char));
    int ptr = 0;
    char c;

    if(cmd==NULL){
        // character array for input command couldn't be allocated so ERROR
        fprintf(stderr, "Couldn't allocate memory for input\n");
        exit(EXIT_FAILURE);
    }

    while(1){
        c = getchar();

        if(ptr>=cmd_size){
            // command is too big
            fprintf(stderr, "Command is too big\n");
            exit(EXIT_FAILURE);
        }

        if(c=='\n'){
            cmd[ptr] = '\0';
            return cmd;
        }
        else{
            cmd[ptr] = c;
        }
        ptr++;
    }
}

struct parsed_cmd* parse_args(char *command){
    int argsize = ARGSIZE;
    char ** args = malloc(argsize*sizeof(char*));
    char *arg;

    int ptr = 0;

    if(args==NULL){
        // memory allocation error
        fprintf(stderr, "Couldn't allocate memory for args parsing\n");
        exit(EXIT_FAILURE);
    }

    arg = strtok(command, " \t\r\a\n");
    while(arg!=NULL){
        // printf("%s ", arg);
        args[ptr] = arg;
        ptr++;

        if(ptr>=argsize){
            argsize += ARGSIZE;
            args = realloc(args, argsize*sizeof(char*));
            if(args==NULL){
                // memory reallocation error
                fprintf(stderr, "Couldn't reallocate memory for args\n");
                exit(EXIT_FAILURE);
            }
        }

        arg = strtok(NULL, " \t\r\a\n");
    }
    args[ptr] = NULL;
    printf("\n");
    struct parsed_cmd *pc = create_cmd(ptr, args);
    return pc;
}

int execute_command(struct parsed_cmd* pc, int builtin_flag){
    char **args = pc->argv;
    if(builtin_flag<0){
        int rc = fork();
        if(rc<0){
            // error in fork
            fprintf(stderr, "fork error\n");
            exit(EXIT_FAILURE);
        }
        else if(rc==0){
            // child process
            // printf("number of arguments you entered is: %d\n", pc->argc);

            // flag to tell if it's a custom command
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
            }
            else{
                // Use inbuilt binary from the OS to execute the command
                printf("This is from binary\n"); //Comment this later
                if(execvp(args[0], args)==-1){
                    // Error in executing command
                    fprintf(stderr, "Error executing this command\n");
                    exit(EXIT_FAILURE);
                }
                else{
                    printf("\n");
                    wait(NULL);
                    // exit(EXIT_SUCCESS);
                    return 0;
                }
            }
        }
    }
    else if(builtin_flag==0){
        int cdr_success = chdir(args[1]);
        if(cdr_success==-1){
            fprintf(stderr, "cd: Enter a valid directory\n");
        }
    }
    else if(builtin_flag==1){
        printf("This custom shell is made by Pushkar The Great\nThis is the help for this shell\n");
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
        printf("\n%s@ %s\n> $ ", username, getcwd(CWD, sizeof(CWD)));
        // taking input
        command = get_input();

        int builtin_flag = -1;

        // parsing input
        pc = parse_args(command);

        for(int i=0; i<3; i++){
            if(strcmp(pc->argv[0], SHELL_COMMANDS[i])==0){
                builtin_flag = i;
            }
        }

        // executing the entered command
        int EXIT_NOW = execute_command(pc, builtin_flag);
        wait(NULL);
        free(command);
        free(pc);
        if(EXIT_NOW==1){
            return;
        }
    }
}

int main(int argc, char *argv[]){
    shell_loop();
    return EXIT_SUCCESS;
}

void custom_pwd(){
    char cwd[256];
    // char *cwd = (char*)malloc(256*sizeof(char));
    if(cwd==NULL){
        fprintf(stderr, "pwd: Error in finding pwd");
        exit(EXIT_FAILURE);
    }
    printf("%s\n", getcwd(cwd, sizeof(cwd)));
    // free(cwd);
    exit(EXIT_SUCCESS);
}

void custom_ls(struct parsed_cmd *pc){
    char *path = ".";
    if(pc->argc > 1){
        path = pc->argv[1];
    }
    DIR *dp;
    if((dp=opendir(path))==NULL){
        fprintf(stderr, "custom_ls: cannot open directory %s\n", path);
        exit(EXIT_FAILURE);
    }
    struct dirent *d;
    while((d=readdir(dp))!=NULL){
        printf("%s\t", d->d_name);
    }
    printf("\n");
    exit(EXIT_SUCCESS);
}