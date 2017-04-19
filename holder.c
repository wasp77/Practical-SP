
char buf[1024];
char* split;
int length = 0;
char** parsed;

int fd = open("test.txt", O_RDONLY);
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






void parseString (char* str, int length) {
  char parsed[length][length + 1];
  int inside = 0;
  int parsed_counter = 0;
  int counter = 0;
  int infile = 0;
  int outfile = 0;
  int arguement_nums[length];
  int args_counter = 0;
  int file_present = 0;

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
    // printf("Got Here parsed_counter: %d counter: %d character: %c \n", parsed_counter, counter, str[n]);
    parsed[parsed_counter][counter++] = str[n];

  }
  parsed[parsed_counter++][counter] = '\0';
  parsed[parsed_counter][0] = '\0';


  for (int num = 0; num < parsed_counter; num++) {
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
