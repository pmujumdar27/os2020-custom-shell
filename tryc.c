#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
  
int main() 
{ 
    char *strings[2] = {"hello", "bro"};

    for(int i=0; i<2; i++){
        printf("%s ", strings[i]);
    }

    printf("\n");

    printf("%s\n", getenv("SESSION_MANAGER"));
  
    return (0); 
} 