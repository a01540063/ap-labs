#include <stdio.h>
#include <stdlib.h>

void month_day(int year, int yearday, int *pmonth, int *pday);
int main(int argc, char ** argv){

    if (argc < 3) {
    printf("You need to put the correct data\n");
    return 1;
  }

    int pmonth = 0;
    int pday = 0;
    int year = atoi(argv[1]);
    int yearday = atoi(argv[2]);
    month_day(year, yearday, &pmonth, &pday);
    printf(" %d, %s\n", pday, argv[1]);
    return 0;
}

void month_day(int year, int yearday, int *pmonth, int *pday){
    static char daytab[2][13] = {{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};
    static char *months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    int i, leap, previousSum, actualSum;
    i = leap = previousSum = actualSum = 0;
    leap = (year%4 == 0 && year%100 != 0) || year%400 ==0;

    for(; yearday > actualSum; i++){
        previousSum = actualSum;
        actualSum += daytab[leap][i];
    }
    *pday = yearday - prevSum;
    *pmonth = i;

    printf("%s", months[i-1]);
}
