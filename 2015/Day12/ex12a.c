#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 40000

int main() {
  FILE *fp = fopen("ex12.input", "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return EXIT_FAILURE;
  }

  char buffer[MAX_LINE];
  fgets(buffer, MAX_LINE, fp);
  fclose(fp);
  char *p    = buffer;
  int length = strlen(buffer);

  int num    = 0;
  int offset = 0;
  int total  = 0;
  while (p - buffer < length) {
    if (sscanf(p, "%d%n", &num, &offset) == 1) { /* read num successfully */
      total += num;
      p     += offset;
    } else { /* failed to read num */
      p++;
    }
  }

  printf("Sum of all nums in input: %d\n", total);
}
