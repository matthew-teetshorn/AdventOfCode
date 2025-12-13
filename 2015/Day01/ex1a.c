#include <stdio.h>

int main() {
  int floor = 0;
  char c;
  FILE *fp = fopen("ex1.input", "r");
  if (fp == NULL) {
    printf("Error reading file\n");
  }

  while ((c = fgetc(fp)) != EOF) {
    if (c == '(') {
      floor++;
    } else if (c == ')') {
      floor--;
    }
  }
  printf("Floor reached: %d\n", floor);
}
