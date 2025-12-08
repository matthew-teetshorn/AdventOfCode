#include <endian.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#define WIDTH 141

int main() {
  FILE *fp;
  char input[WIDTH + 2];
  int currBeams[WIDTH] = {0};
  uint64_t pathsToBeam[WIDTH] = {0};
  uint64_t total = 0;

  fp = fopen("ex7.input", "r");

  clock_t start = clock();

  while (fgets(input, WIDTH + 2, fp) != NULL) {
    for (int i = 0; i < WIDTH; i++) {
      if (input[i] == 'S') {
        currBeams[i] = 1;
        pathsToBeam[i] = 1;
      }
      if (input[i] == '^' && currBeams[i] == 1) {
        if (i > 0) {
          currBeams[i - 1] = 1;
          pathsToBeam[i - 1] += pathsToBeam[i];
        }
        if (i < WIDTH - 1) {
          currBeams[i + 1] = 1;
          pathsToBeam[i + 1] += pathsToBeam[i];
        }
        currBeams[i] = 0;
        pathsToBeam[i] -= pathsToBeam[i];
      }
    }
  }

  for (int i = 0; i < WIDTH; i++) {
    total += pathsToBeam[i];
  }

  clock_t end = clock();

  printf("Total splits: %" PRIu64 "\n", total);

  double elapsed = ((double)(end - start) / CLOCKS_PER_SEC);
  printf("Total time: %f\n", elapsed);
}
