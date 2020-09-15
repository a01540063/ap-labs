#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

void analizeLog(char *logFile, char *report);
char* getWord(char *word);
char* getDate(char *date);
char* getLine(int l);

struct Package {
  char* name;
  char* installDate;
  char* updateDate;
  int updates;
  char* removalDate;
};

struct Package pkgs[1000];
int installed, upgraded, removed, current;


int main(int argc, char **argv) {

    if (argc != 5) {
	     printf("Usage: ./pacman-analizer.o -input\n");
	      return 1;
    }
    analizeLog(argv[2], argv[4]);

    return 0;
}

void analizeLog(char *logFile, char *report) {
  printf("Generating Report from: [%s] \n", logFile);
  // Implement your solution here.

  int r = open(logFile,O_RDONLY);
  if (r == -1) {
    printf("Failed to open and read the log file! \n");
    exit(1);
  }

  char *line;
  while((line = getLine(r)) != NULL){
    char *option = getWord(line+26);
    if (strcmp("installed", option) == 0) {
      pkgs[installed].name = getWord(line+36);
      pkgs[installed].installDate = getDate(line+1);
      installed++;
      current++;
    }
    else if (strcmp("upgraded", option) == 0) {
      char *name =getWord(line+35);
      for (int i = 0; i < installed; i++) {
        if (strcmp(pkgs[i].name, name) == 0) {
          pkgs[i].updateDate = getDate(line+1);
          pkgs[i].updates++;
          upgraded++;
        }
      }
    }
    else if (strcmp("removed", option) == 0) {
      char *name =getWord(line+34);
      for (int i = 0; i < installed; i++) {
        if (strcmp(pkgs[i].name, name) == 0) {
          pkgs[i].removalDate = getDate(line+1);
          removed++;
          current--;
        }
      }
    }
  }

  for (int i = 0; i < installed; i++) {
    if(pkgs[i].removalDate == NULL){
      pkgs[i].removalDate = "-";
    }
    if(pkgs[i].updateDate == NULL){
      pkgs[i].updateDate = "-";
    }
  }
  close(r);

  int w = open(report, O_WRONLY | O_CREAT, 0600);
  if (w == -1) {
    printf("Failed to open and write the report file! \n");
    exit(1);
  }
  char text[100];

  char* title = "Pacman Packages Report\n----------------------\n";
  write(w, title, strlen(title));
  sprintf(text, "%s%d\n", "- Installed packages : ", installed);
  write(w, text, strlen(text));
  sprintf(text, "%s%d\n", "- Removed packages : ", removed);
  write(w, text, strlen(text));
  sprintf(text, "%s%d\n", "- Upgraded packages : ", upgraded);
  write(w, text, strlen(text));
  sprintf(text, "%s%d\n", "- Current installed : ", current);
  write(w, text, strlen(text));

  char* secondTitle = "\nList of packages\n----------------\n";
  write(w, secondTitle, strlen(secondTitle));

  for (int i = 0; i < installed; i++) {
     sprintf(text, "%s%s\n", "- Package Name : ", pkgs[i].name);
     write(w,text,strlen(text));
     sprintf(text, "%s%s\n", "     - Install date : ", pkgs[i].installDate);
     write(w,text,strlen(text));
     sprintf(text, "%s%s\n", "     - Last update date : ", pkgs[i].updateDate);
     write(w,text,strlen(text));
     sprintf(text, "%s%d\n", "     - How many updates : ", pkgs[i].updates);
     write(w,text,strlen(text));
     sprintf(text, "%s%s\n", "     - Removal date : ", pkgs[i].removalDate);
     write(w,text,strlen(text));
  }

  close(w);

  printf("Report is generated at: [%s]\n", report);
}

char* getWord(char *word){
  int size;
  for(int i=0; i<strlen(word); i++){
    if(word[i] == ' ' || word[i] == '\n'){
      size = i;
      break;
    }
  }
  char *wordRes = calloc(size+1,1);
  for(int i=0; i<size; i++){
      wordRes[i] = word[i];
  }
  wordRes[size] = '\0';
  return wordRes;
}

char* getDate(char *date){
  int size;
  for(int i=0;i<strlen(date);i++){
    if(date[i] == ']'){
      size = i;
      break;
    }
  }
  char *dateRes = calloc(size+1,1);
  for(int i=0; i<size; i++){
    dateRes[i] = date[i];
  }
  dateRes[size] = '\0';
  return dateRes;
}

char* getLine(int l){
  int size = 100;
  int total = 0;
  char *buffer;
  buffer = calloc(size, 1);
  int n;

  while((n = read(l, buffer+total, size)) > 0){
    for(int i=total; i<total+n; i++){
      if(buffer[i] == '\n'){
        lseek(l, i-(total+n)+1, SEEK_CUR);
        buffer[i] = '\0';
        return buffer;
      }
    }

    total+=n;
    buffer = realloc(buffer, total+size);
  }
  if(total>0){
    buffer[total] = '\0';
    return buffer;
  }

  return NULL;
}
