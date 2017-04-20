#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>
#include "systems_programming.h"
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char **argv) {
  char* line = NULL;
  size_t size;
  int length = 0;
  char** parsed;
  int child;
  int my_pipe[2];
  char pipe_buf[1024];

  while (getline(&line, &size, stdin) != -1) {
    length = strlen(line);
    parsed = make2DArray(length);
    parsed = parseString(line, length, parsed);


    pipe(my_pipe);
    child = fork();
    if (child<0) {perror("Forking failed"); exit(1);}
    if (child == 0) {
      executeCommand(parsed, length, my_pipe);
    } else {
      close(my_pipe[1]);
      while (1) {
        for (int j = 0; j < 1024; j++) {
          pipe_buf[j] = '\0';
        }
        int jbytes = read(my_pipe[0], &pipe_buf, sizeof(pipe_buf));
        if(jbytes<0) {perror("Read failed: test.txt"); exit(1);}
        if (jbytes == 0) {
          for (int j = 0; j < 1024; j++) {
            pipe_buf[j] = '\0';
          }
          break;
        }
        printf("%s", pipe_buf);
      }
      int stat_loc;
      wait(&stat_loc);
      // printf("Child (id = %i) exit status:%i\n" , cpid, WEXITSTATUS(stat_loc));
      free2DArray(parsed, length);
    }


  }
}

void executeCommand(char** parsed, int length, int* my_pipe) {
  int infile = 0;
  int outfile = 0;
  int arguement_nums[length];
  int args_counter = 0;
  int file_present = 0;
  char* program;


  for (int num = 0; parsed[num][0] != '\0'; num++) {
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

  if (infile > 0) {
    int in = open(parsed[infile],O_RDONLY);
    if (in<0) {
      printf("Read failed: %s\n", parsed[infile]);
      exit(1);
    } else {
      dup2 (in, 0);
      close(my_pipe[0]);
      close(my_pipe[1]);
    }
  } else if (outfile > 0) {
    int out = open(parsed[outfile],O_CREAT|O_WRONLY|O_APPEND,S_IRWXU);
    if (out<0) {
      printf("Write failed: %s\n", parsed[outfile]);
      exit(1);
    } else {
      dup2 (out, 1);
      close(my_pipe[0]);
      close(my_pipe[1]);
    }
  } else {
    dup2 (my_pipe[1], 1);
    close(my_pipe[0]);
    close(my_pipe[1]);
  }


  char* arguements[args_counter + 2];
  int counter = 0;
  arguements[counter++] = program;

  for (int x = 0; x < args_counter; x++) {
    int arg = arguement_nums[x];
    arguements[counter] = parsed[arg];
    counter++;
  }
  arguements[counter] = NULL;

  if (execv(program, arguements) < 0) {
    printf("Execute failed: %s\n", program);
  }

}
