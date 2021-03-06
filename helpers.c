#include "systems_programming.h"
#include <stdlib.h>

// Alocate memory for a 2D array
char** make2DArray (int length) {
  int column_length = length + 1;
  char** parsed = (char**)malloc(length * sizeof(char *));
  for (int t = 0; t < length; t++) {
    parsed[t] = (char *)malloc(column_length * sizeof(char));
  }
  return parsed;
}

// Free up the memory for a 2D array
void free2DArray (char** parsed, int length) {
  for (int count = 0; count < length; count++) {
    free(parsed[count]);
  }
}
