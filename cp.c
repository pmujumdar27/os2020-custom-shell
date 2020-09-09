#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<dirent.h>
#include<string.h>
#include<errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "globals.h"

// function to check if given argument is a complete path or relative path and return folder name if it's a complete path
char *get_dir_name(char *path){
    char *token;
    char *directory;
    size_t length;

    // printf("path to get dir_name: %s\n", path); //debugging 

    token = strrchr(path, '/'); // finds last occurence of character and tokenizes

    if(token==NULL){
        return path;
    }

    length = strlen(token);
    directory = malloc(length);
    memcpy(directory, token+1, length);

    // printf("copying from folder: %s\n", (char*)directory); //debugging 

    return (char*)directory;
}

// helper function to copy a single file
void copy_file(char *src, char *dst){
    FILE *fpsrc, *fpdst;
    int c;
    
    // opening the source and dest files and writing content read from source to the dest
    if((fpsrc = fopen(src, "r")) && (fpdst=fopen(dst, "w"))){
        while((c=fgetc(fpsrc))!=EOF){
            fputc(c, fpdst);
        }
        fclose(fpsrc);
        fclose(fpdst);
    }
    else{
        fprintf(stderr, "cp-copying single file: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void custom_cp(struct parsed_cmd *pc){
    if(pc->argc==3){
        copy_file(pc->argv[1], pc->argv[2]);
    }
    else if(pc->argc==4 && (strcmp(pc->argv[1], "-r")==0)){
        DIR *dpsrc;
        // opening source directory
        if((dpsrc=opendir(pc->argv[2]))==NULL){
            fprintf(stderr, "cp-opening directory: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        char *dest_path = pc->argv[3]; // path of the destination directory
        char *cp_dir_name = get_dir_name(pc->argv[2]);
        strcat(dest_path, "/");
        strcat(dest_path, cp_dir_name);
        
        // making folder with the same name as source folder in the destination directory
        if(mkdir(dest_path, 0777)==-1){
            fprintf(stderr, "cp-mkdir: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        struct dirent *d;
        while((d=readdir(dpsrc))!=NULL){
            char *src_path = strdup(pc->argv[2]);
            strcat(src_path, "/");
            strcat(src_path, d->d_name);

            // checking if current path is of a directory or a file, copy it if it's a file
            struct stat statbuf;
            stat(src_path, &statbuf);
            // printf("src_path: %s\n", src_path); //debugging 
            if(S_ISREG(statbuf.st_mode)){
                char *dest_file_path = strdup(dest_path);
                strcat(dest_file_path, "/");
                strcat(dest_file_path, get_dir_name(src_path));
                copy_file(src_path, dest_file_path);
            }
        }
    }
    else if(strcmp(pc->argv[1], "-r")==0){
        fprintf(stderr, "cp: -r <source_dir> <dest_dir>\n");
        exit(EXIT_FAILURE);
    }
    else{
        fprintf(stderr, "cp: <source_dir> <dest_dir>\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}