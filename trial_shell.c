#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<dirent.h>

#define MAX_COMM_SIZE 2000
#define ARGSIZE 100

char CWD[256];

char *shell_commands[3] = {"cd", "help", "exit"};

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

void execute_command(struct parsed_cmd* pc, int builtin_flag){
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
            if(execvp(args[0], args)==-1){
                // Error in executing command
                fprintf(stderr, "Error executing this command\n");
                exit(EXIT_FAILURE);
            }
            else{
                printf("\n");
                wait(NULL);
                // printf("\n");
                return;
            }
        }
    }
    else if(builtin_flag==0){
        chdir(args[1]);
    }
    else if(builtin_flag==1){
        printf("This custom shell is made by Pushkar The Great\nThis is the help for this shell\n");
    }
    else if(builtin_flag==2){
        printf("Exiting ...\n");
        EXIT_SHELL = 1;
    }
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
            if(strcmp(pc->argv[0], shell_commands[i])==0){
                builtin_flag = i;
            }
        }

        // executing the entered command
        execute_command(pc, builtin_flag);
        wait(NULL);
        free(command);
        free(pc);
    }
}

int main(int argc, char *argv[]){
    shell_loop();
    return EXIT_SUCCESS;
}