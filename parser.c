#include "systems_programming.h"
#include <stdlib.h>

// Return a 2D array containing the strings for output
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
    if (str[n] == ' ') { // If space is inside quotes ignore otherwise enter a null character
      if (inside == 0) {
        parsed[parsed_counter][counter] = '\0';
        counter = 0;
        parsed_counter++;
        continue;
      }
    }
    if (str[n] == '\n') { // Don't want newline characters in the parsed string
      parsed[parsed_counter][counter] = '\0';
      continue;
    }
    parsed[parsed_counter][counter++] = str[n];

  }
  parsed[parsed_counter++][counter] = '\0'; // Make sure all strings end with null character 
  parsed[parsed_counter][0] = '\0';

  return parsed;
}
