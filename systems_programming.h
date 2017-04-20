
char** make2DArray (int length);
void free2DArray (char** parsed, int length);
char** parseString (char* str, int length, char** parsed);
void printContents(char** parsed, int length);
void executeCommand(char** parsed, int length, int* my_pipe);
