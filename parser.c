#include "systems_programming.h"
#include <stdlib.h>

char** parseString (char* str, int length, char** parsed) {
  int inside = 0;
  int parsed_counter = 0;
  int counter = 0;

  for (int n = 0; n < length; n++) {
    if (str[n] == '"') {
      if (inside == 1) {
        inside = 0;
        continue;
      } else {
        inside = 1;
        continue;
      }
    }
    if (str[n] == ' ') {
      if (inside == 0) {
        parsed[parsed_counter][counter] = '\0';
        counter = 0;
        parsed_counter++;
        continue;
      }
    }
    if (str[n] == '\n') {
      continue;
    }
    // printf("Got Here parsed_counter: %d counter: %d character: %c \n", parsed_counter, counter, str[n]);
    parsed[parsed_counter][counter++] = str[n];

  }
  parsed[parsed_counter++][counter] = '\0';
  parsed[parsed_counter][0] = '\0';

  return parsed;
}
