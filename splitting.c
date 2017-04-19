#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>
#include "systems_programming.h"

int main(int argc, char **argv) {
  char* file = argv[0];
  char buf[1024];
  char* split;
  int length = 0;
  char** parsed;

  int fd = open(file, O_RDONLY);
  if(fd<0) {perror("open failed:"); exit(1);}
  int bytes = read(fd, &buf, sizeof(buf) - 1);
  buf[bytes] = '\0';

  split = strtok(buf, "\n");
  while (split != NULL) {
    length = strlen(split);
    parsed = make2DArray(length);
    parsed = parseString(split, length, parsed);
    printContents(parsed, length);
    free2DArray(parsed, length);
    split = strtok(NULL, "\n");
  }
}


void printContents(char** parsed, int length) {
  int infile = 0;
  int outfile = 0;
  int arguement_nums[length];
  int args_counter = 0;
  int file_present = 0;


  for (int num = 0; parsed[num][0] != '\0'; num++) {
      if (num == 0) {
        printf("Run program \"%s\" ", parsed[num]);
        continue;
      }
      if (parsed[num][0] == '<') {
        infile = num + 1;
        file_present = 1;
        continue;
      }
      if (parsed[num][0] == '>') {
        outfile = num + 1;
        file_present = 1;
        continue;
      }
      if (file_present == 1) {
        file_present = 0;
        continue;
      }
      arguement_nums[args_counter++] = num;
  }

  if (args_counter == 1) {
    printf("with the arguement ");
  }
  if (args_counter > 1){
    printf("with the arguements ");
  }

  for (int x = 0; x < args_counter; x++) {
    int arg = arguement_nums[x];
    if ((x + 1) == args_counter) {
      printf("\"%s\". ", parsed[arg]);
    } else {
      printf("\"%s\" and ", parsed[arg]);
    }
  }

  if (infile > 0) {
    printf("Read the input from the file \"%s\". ", parsed[infile]);
  }
  if (outfile > 0) {
    printf("Write the output to the file \"%s\". ", parsed[outfile]);
  }

  printf("\n");
}
