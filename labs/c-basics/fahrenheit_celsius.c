#include <stdio.h>
#include <stdlib.h>

#define   LOWER  0       /* lower limit of table */
#define   UPPER  300     /* upper limit */
#define   STEP   20      /* step size */

/* print Fahrenheit-Celsius table */

int main(int argc, char *argv[]){
  if (argc == 2){
    printf("Fahrenheit: %3d, Celcius: %6.1f\n", atoi(argv[1]), (5.0/9.0)*(atoi(argv[1])-32));


  } else if (argc == 4){
    int fahr, start, end, step;
    start = atoi(argv[1]);
    end = atoi(argv[2]);
    step = atoi(argv[3]);

    for (fahr = start; fahr <= end; fahr = fahr + step)
    printf("Fahrenheit: %3d, Celcius: %6.1f\n", fahr, (5.0/9.0)*(fahr-32));

  }
  return 0;
}
