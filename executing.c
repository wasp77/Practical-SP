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
  char* file = argv[0];
  char buf[1024];
  char* split;
  int length = 0;
  char** parsed;
  int child;
  int my_pipe[2];
  char pipe_buf[1024];

  int fd = open(file, O_RDONLY);
  if(fd<0) {perror("Read failed: test.txt"); exit(1);}
  int bytes = read(fd, &buf, sizeof(buf) - 1);
  if(bytes<0) {perror("Read failed: test.txt"); exit(1);}
  buf[bytes] = '\0';
  split = strtok(buf, "\n");

  while (split != NULL) {
    length = strlen(split);
    parsed = make2DArray(length);
    parsed = parseString(split, length, parsed);


    pipe(my_pipe);
    child = fork();
    if (child<0) {perror("Forking failed"); exit(1);}
    if (child == 0) {
      dup2 (my_pipe[1], 1);
      close(my_pipe[0]);
      close(my_pipe[1]);
      executeCommand(parsed, length);
    } else {
      close(my_pipe[1]);
      while (1) {
        int jbytes = read(my_pipe[0], &pipe_buf, sizeof(pipe_buf) - 1);
        if(jbytes<0) {perror("Read failed: test.txt"); exit(1);}
        if (jbytes == 0) {
          for (int j = 0; j < 1024; j++) {
            pipe_buf[j] = '\0';
          }
          break;
        }
        printf("%s", pipe_buf);
        for (int j = 0; j < 1024; j++) {
          pipe_buf[j] = '\0';
        }
      }
      int stat_loc;
      pid_t cpid = wait(&stat_loc);
      printf("Child (id = %i) exit status:%i\n" , cpid, WEXITSTATUS(stat_loc));
      free2DArray(parsed, length);
      split = strtok(NULL, "\n");
    }


  }
}

void executeCommand(char** parsed, int length) {
  // int infile = 0;
  // int outfile = 0;
  int arguement_nums[length];
  int args_counter = 0;
  int file_present = 0;
  char* program;


  for (int num = 0; parsed[num][0] != '\0'; num++) {
      if (num == 0) {
        program = parsed[num];
        continue;
      }
      // if (parsed[num][0] == '<') {
      //   infile = num + 1;
      //   file_present = 1;
      //   continue;
      // }
      // if (parsed[num][0] == '>') {
      //   outfile = num + 1;
      //   file_present = 1;
      //   continue;
      // }
      if (file_present == 1) {
        file_present = 0;
        continue;
      }
      arguement_nums[args_counter++] = num;
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
    printf("%s\n", strerror(errno));
  }

}
