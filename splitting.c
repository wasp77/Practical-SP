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
  char* line = NULL;
  size_t size;
  int length = 0;
  char** parsed;

  while (getline(&line, &size, stdin) != -1) { // Readline in from stdin to be parsed
    length = strlen(line);
    parsed = make2DArray(length);
    parsed = parseString(line, length, parsed);
    printContents(parsed, length);
    free2DArray(parsed, length);
  }
}


void printContents(char** parsed, int length) {
  int infile = 0;
  int outfile = 0;
  int arguement_nums[length];
  int args_counter = 0;
  int file_present = 0;
  char* program;


  for (int num = 0; parsed[num][0] != '\0'; num++) { // Iterate over the parsed 2D array checking for input and output files
      if (num == 0) {
        program = parsed[num];
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
    printf("Run program \"%s\"", program);
    printf(" with argument ");
  } else if (args_counter > 1){
    printf("Run program \"%s\"", program);
    printf(" with arguments ");
  } else {
    printf("Run program \"%s\".", program);
  }

  for (int x = 0; x < args_counter; x++) { // Output all the arguments
    int arg = arguement_nums[x];
    if ((x + 1) == args_counter) {
      printf("\"%s\".", parsed[arg]); // The last argument
    } else {
      printf("\"%s\" and ", parsed[arg]);
    }
  }

  // Output files if they exist
  if (infile > 0) {
    printf(" Read the input from file \"%s\".", parsed[infile]);
  }
  if (outfile > 0) {
    printf(" Write the output to file \"%s\".", parsed[outfile]);
  }
}
