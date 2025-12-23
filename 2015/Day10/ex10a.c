#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE   20
#define RESULT_LEN 5242880 /* 5 MB buffer to store result */
#define NUM_TRIES  50

/* Won't fit on stack */
char result[RESULT_LEN];
char temp[RESULT_LEN];

int main() {
  FILE *fp = fopen("ex10.input", "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return EXIT_FAILURE;
  }

  char buffer[MAX_LINE];
  fgets(buffer, MAX_LINE, fp);
  fclose(fp);

  size_t len = strlen(buffer);
  if (len > 0 && buffer[len - 1] == '\n')
    buffer[len - 1] = '\0'; /* strip newline */
  strcpy(result, buffer);

  for (int i = 0; i < NUM_TRIES; i++) {
    char prevNum = result[0];
    int count    = 1;
    int offset   = 0;

    int numchars = strlen(result);
    for (int j = 1; j < numchars; j++) {
      if (result[j] == prevNum) {
        count++;
      } else {
        offset  += sprintf(temp + offset, "%d%c", count, prevNum);
        prevNum  = result[j];
        count    = 1;
      }
    }
    offset += sprintf(temp + offset, "%d%c", count, prevNum);
    strcpy(result, temp);
  }
  printf("Result length: %zu\n", strlen(result));
}
