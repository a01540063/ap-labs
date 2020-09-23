#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int numcmp(char *s1, char *s2) {
    double val1, val2;
    val1 = atof(s1);
    val2 = atof(s2);

    if(val1 < val2) {
        return -1;
    }
    else if(val1 > val2){
        return 1;
    }
    return 0;
}

void merge(char *lineptr[], int l, int m, int r, int (*comp)(void *,void *)){
	int i,j,k;
	int s1 = m-l+1;
	int s2 = r-m;

	void *left[s1];
	void *right[s2];
	for(i=0; i<s1; i++){
		left[i] = lineptr[l+i];
	}
	for(i=0; i<s2; i++){
		right[i] = lineptr[m+i+1];
	}

	i=0;j=0;k=l;
    while(i<s1 && j<s2){
    	if(comp(left[i],right[j]) < 0){
			lineptr[k++] = left[i++];
		}
		else{
			lineptr[k++] = right[j++];
		}
	}

	if(i==s1){
		while(j<s2){
			lineptr[k++] = right[j++];
		}
	}
	else{
		while(i<s1){
			lineptr[k++] = left[i++];
		}
	}
}

void mergeSort(char *lineptr[], int start, int end, int (*comp)(void *,void *)){
	int middle = start+(end-start)/2;
	if(end>start){
		mergeSort(lineptr, start, middle, comp);
		mergeSort(lineptr, middle+1, end, comp);
		merge(lineptr, start, middle, end, comp);
	}
}



int main(int argc, char **argv){
  char *file;
  int flag = 0;
  if (argc == 2){
      file = argv[1];
  }
  else if (argc == 3 && strcmp(argv[1], "-n")==0){
      file = argv[2];
      flag =1;
  }
  else{
      printf("Missing arguments! \n");
      return 0;
  }

  int arraySize = 100;
  char **arrayList = malloc(sizeof(char *) * arraySize);

  FILE *fp = fopen(file, "r");
  if (!fp){
      printf("Error, Failed to open the file! \n");
      return 0;
  }

  char *line_buf = NULL;
  size_t line_buf_size = 0;
  int line_count = 0;
  ssize_t line_size;

  line_size = getline(&line_buf, &line_buf_size, fp);
  while (line_size >= 0){
    arrayList[line_count] = malloc(sizeof(char) * line_buf_size);

    sprintf(arrayList[line_count], "%s", line_buf);
    arrayList[line_count][strlen(line_buf) - 1] = 0;
    line_count++;
    line_size = getline(&line_buf, &line_buf_size, fp);
  }

  free(line_buf);
  fclose(fp);

  printf("Original: \n");
  for (int i = 0; i < arraySize; i++) {
    printf("%s\n",arrayList[i]);
  }

  if(flag == 0){
    mergeSort(arrayList, 0, arraySize - 1, (int (*)(void *, void *)) strcmp);
  }
  else if(flag ==1){
    mergeSort(arrayList, 0, arraySize - 1, (int (*)(void *, void *)) numcmp);
  }


  printf("\nOrdered: \n");
  for (int i = 0; i < arraySize; i++) {
    printf("%s\n",arrayList[i]);
  }

  return 0;
}
