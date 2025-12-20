#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE 50

int main() {
  FILE *fp = fopen("ex8.input", "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return EXIT_FAILURE;
  }

  char buffer[MAX_LINE];
  char *p;
  int long_count, short_count, total;
  total = 0;
  while (fgets(buffer, MAX_LINE, fp) != NULL) {
    p          = buffer;
    long_count = short_count = 0;

    while (*p != '\n') {
      switch (*p) {
      case '\\':
        p++;
        long_count++;
        if (*p == 'x') {
          p          += 3;
          long_count += 3;
          short_count++;
        } else {
          p++;
          long_count++;
          short_count++;
        }
        break;
      case '\"':
        long_count++;
        p++;
        break;
      default:
        long_count++;
        short_count++;
        p++;
      }
    }

    total += long_count - short_count;
  }

  printf("Total difference: %d\n", total);
  fclose(fp);
}
