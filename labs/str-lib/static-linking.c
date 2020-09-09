#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int mystrlen(char *);
char *mystradd(char *, char *);
int mystrfind(char *, char *);

int main(int argc, char* argv[]) {

    if (argc != 4){
        printf("Please use correct arguments. \n");
        return 0;
    }

    if(strcmp(argv[1], "-add") == 0){
        int len = mystrlen(argv[2]);
        char* newStr = mystradd(argv[2], argv[3]);
        int newLen = mystrlen(newStr);
        printf("Initial Length      : %i\n", len);
        printf("New String             : %s\n", newStr);
        printf("New Length          : %i\n", newLen);
    }else if(strcmp(argv[1], "-find") == 0){
        int pos = mystrfind(argv[2], argv[3]);
        printf("['%s'] string was found at [%i] position\n", argv[3], pos);
    }else{
        printf("Please use -add or -find as the second argument\n");
        return 0;
    }

    return 0;
}
