#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 35
#define DIM      1000
#define TOGGLE   0
#define TURNON   1
#define TURNOFF  2

int main() {
  FILE *fp = fopen("ex6.input", "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return EXIT_FAILURE;
  }

  int operation;
  char *on           = "turn on";
  char *off          = "turn off";
  char *toggle       = "toggle";
  int grid[DIM][DIM] = {0};
  char buffer[MAX_LINE];
  char *bufPtr;
  int x1, y1, x2, y2;

  while (fgets(buffer, MAX_LINE, fp) != NULL) {
    bufPtr = buffer;
    if (!strncmp(buffer, on, strlen(on))) {
      operation  = TURNON;
      bufPtr    += strlen(on);
    } else if (!strncmp(buffer, off, strlen(off))) {
      operation  = TURNOFF;
      bufPtr    += strlen(off);
    } else if (!strncmp(buffer, toggle, strlen(toggle))) {
      operation  = TOGGLE;
      bufPtr    += strlen(toggle);
    }

    sscanf(bufPtr, "%d,%d through %d,%d", &x1, &y1, &x2, &y2);

    for (int i = x1; i <= x2; i++) {
      for (int j = y1; j <= y2; j++) {
        switch (operation) {
        case TOGGLE:
          grid[i][j] ^= 1;
          break;
        case TURNON:
          grid[i][j] = 1;
          break;
        case TURNOFF:
          grid[i][j] = 0;
          break;
        }
      }
    }
  }

  int total = 0;
  for (int i = 0; i < DIM; i++) {
    for (int j = 0; j < DIM; j++) {
      if (grid[i][j]) ++total;
    }
  }

  printf("Total lights on: %d\n", total);
  fclose(fp);
}
