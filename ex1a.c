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
  int count = 0;

  fp = fopen("ex1.input", "r");
  if (fp == NULL) {
    perror("Error opening file");
    return EXIT_FAILURE;
  }

  while (fgets(buffer, MAX_LINE_LENGTH, fp) != NULL) {
    dir = buffer[0];
    sscanf(&buffer[1], "%d", &rot);

    if (dir == 'L') {
      rot *= -1;
    }
    pos = (pos + rot) % DIAL_SIZE;

    if (pos == 0) {
      ++count;
    }
  }

  printf("Combination: %d\n", count);
  fclose(fp);
}
