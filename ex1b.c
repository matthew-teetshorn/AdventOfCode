#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 10
#define DIAL_SIZE 100

int main() {
  FILE *fp;
  char buffer[MAX_LINE_LENGTH];
  char dir;
  int rot = 0;
  int pos = 50;
  int last_pos = 50;
  int count = 0;
  int incr = 1;

  fp = fopen("ex1.input", "r");
  if (fp == NULL) {
    perror("Error opening file");
    return EXIT_FAILURE;
  }

  while (fgets(buffer, MAX_LINE_LENGTH, fp) != NULL) {
    dir = buffer[0];
    sscanf(&buffer[1], "%d", &rot);

    if (dir == 'L') {
      incr = -1;
    } else if (dir == 'R') {
      incr = 1;
    }

    // Very naive implementation, move every click
    for (int rcount = 0; rcount < rot; ++rcount) {
      pos += incr;
      if (pos % DIAL_SIZE == 0) {
        ++count;
      }
    }
  }

  printf("Combination: %d\n", count);
  fclose(fp);
}
