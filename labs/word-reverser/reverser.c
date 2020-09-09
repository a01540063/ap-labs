#include <stdio.h>

int main(){
    // Place your magic here
    char c;
    char word[100];

    int i =0;
    while ((c = getchar()) != EOF) {
      if (c== '\n') {
        i--;
        while (i>= 0) {
          printf("%c", word[i--]);
        }
        i = 0;
        printf("\n");
      }
      else{
        word[i++] = c;
      }
    }
    return 0;
}
