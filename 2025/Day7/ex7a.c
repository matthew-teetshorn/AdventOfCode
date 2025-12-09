#include <endian.h>
#include <stdio.h>

#define WIDTH 141

int main() {
  FILE *fp;
  char input[WIDTH + 2];
  int beams[WIDTH] = {0};
  int total = 0;

  fp = fopen("ex7.input", "r");

  while (fgets(input, WIDTH + 2, fp) != NULL) {
    for (int i = 0; i < WIDTH; i++) {
      // printf("%c", input[i]);
      if (input[i] == 'S') {
        beams[i] = 1;
      }
      if (input[i] == '^' && beams[i] == 1) {
        total++;
        if (i > 0) {
          beams[i - 1] = 1;
        }
        if (i < WIDTH - 1) {
          beams[i + 1] = 1;
        }
        beams[i] = 0;
      }
    }
  }

  printf("Total splits: %d\n", total);
}
