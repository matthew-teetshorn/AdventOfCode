#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define DIM        11
#define NUM_POINTS 495

typedef struct {
  int x;
  int y;
} Coord_t;

int main() {
  FILE *fp = fopen("ex9.input", "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return EXIT_FAILURE;
  }

  char buffer[DIM + 2];
  Coord_t coords[NUM_POINTS];

  for (int i = 0; i < NUM_POINTS; i++) {
    if (fgets(buffer, DIM + 2, fp) == NULL) {
      printf("Error reading from file to buffer\n");
      return EXIT_FAILURE;
    }

    sscanf(buffer, "%d,%d", &coords[i].x, &coords[i].y);
  }
  fclose(fp);

  uint64_t currArea;
  uint64_t maxArea = 0;
  for (int i = 0; i < NUM_POINTS; i++) {
    for (int j = i + 1; j < NUM_POINTS; j++) {
      Coord_t p1 = {coords[i].x, coords[i].y};
      Coord_t p2 = {coords[j].x, coords[j].y};
      currArea =
          (uint64_t)(abs(p1.x - p2.x) + 1) * (uint64_t)(abs(p1.y - p2.y) + 1);
      if (currArea > maxArea) {
        maxArea = currArea;
      }
    }
  }

  printf("Max area: %" PRIu64 "\n", maxArea);
}
