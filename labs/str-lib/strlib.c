int mystrlen(char *str);
char *mystradd(char *origin, char *addition);
int mystrfind(char *origin, char *substr);


int mystrlen(char *str){
    int i = 0;

    while(str[i] != '\0'){
        i++;
    }
    return i;
}

char *mystradd(char *origin, char *addition){

    int originLen = mystrlen(origin);
    int i;
    for(i = 0; addition[i] != '\0'; i++){
        origin[originLen + i] = addition[i];
    }
    origin[originLen + i] = '\0';
    return origin;
}

int mystrfind(char *origin, char *substr){
    int len = mystrlen(origin);
    int sublen = mystrlen(substr);
    int i, j;

    for(i=0; i<len;i++){
        if(origin[i] == substr[0]){
            for (j=1;j<sublen;j++){
                if (origin[i+j] != substr[j])
                    break;
            }
            if(j == sublen)
                return i;
        }
    }
    return -1;
}
