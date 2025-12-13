#include <stdio.h>

int main() {
  int floor = 0;
  int count = 0;
  char c;
  FILE *fp = fopen("ex1.input", "r");
  if (fp == NULL) {
    printf("Error reading file\n");
  }

  while ((c = fgetc(fp)) != EOF) {
    count++;
    if (c == '(') {
      floor++;
    } else if (c == ')') {
      floor--;
    }
    if (floor < 0) {
      break;
    }
  }
  printf("Floor reached: %d\n", floor);
  printf("Basement reached at step: %d\n", count);
}
