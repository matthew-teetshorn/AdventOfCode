#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 100

int main() {
  FILE *fp = fopen("ex8.input", "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return EXIT_FAILURE;
  }

  char old_string[MAX_LINE];
  char new_string[MAX_LINE];
  char *o, *n;
  int old_count, new_count, total;
  total = 0;
  while (fgets(old_string, MAX_LINE, fp) != NULL) {
    o = old_string;
    n = new_string;

    *n++ = '"';
    while (*o != '\n') {
      switch (*o) {
      case '\\':
      case '\"':
        *n++ = '\\';
      default:
        *n++ = *o++;
      }
    }
    *n++ = '"';
    *o = *n   = '\0';
    old_count = strlen(old_string);
    new_count = strlen(new_string);

    total += new_count - old_count;
  }

  printf("Total difference: %d\n", total);
  fclose(fp);
}
