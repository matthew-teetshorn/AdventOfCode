#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define DIM 11
#define NUM_POINTS 496

typedef struct {
  int x;
  int y;
} Coord_t;

int compareInts(const void *a, const void *b) {
  int aVal = *(const int *)a;
  int bVal = *(const int *)b;

  if (aVal < bVal) {
    return -1;
  }
  if (aVal == bVal) {
    return 0;
  }
  return 1;
}

void floodFill(int buffer[300][300], int x, int y) {
  if (buffer[y][x]) {
    return;
  }
  buffer[y][x] = 1;
  floodFill(buffer, x + 1, y);
  floodFill(buffer, x - 1, y);
  floodFill(buffer, x, y + 1);
  floodFill(buffer, x, y - 1);
}

int isInside(int buffer[300][300], Coord_t p1, Coord_t p2) {
  int startX = p1.x < p2.x ? p1.x : p2.x;
  int startY = p1.y < p2.y ? p1.y : p2.y;
  int endX = p1.x > p2.x ? p1.x : p2.x;
  int endY = p1.y > p2.y ? p1.y : p2.y;

  for (int y = startY; y <= endY; y++) {
    for (int x = startX; x <= endX; x++) {
      // printf("(%d, %d)\n", y, x);
      if (!buffer[y][x]) {
        return 0;
      }
    }
  }
  return 1;
}

int main() {
  FILE *fp = fopen("ex9.input", "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return EXIT_FAILURE;
  }

  char buffer[DIM + 2];
  Coord_t coords[NUM_POINTS];
  Coord_t compressed[NUM_POINTS];
  int xs[NUM_POINTS];
  int ys[NUM_POINTS];
  int uxs[NUM_POINTS];
  int uys[NUM_POINTS];
  int uniqueX = 0;
  int uniqueY = 0;

  for (int i = 0; i < NUM_POINTS; i++) {
    if (fgets(buffer, DIM + 2, fp) == NULL) {
      printf("Error reading from file to buffer\n");
      return EXIT_FAILURE;
    }

    sscanf(buffer, "%d,%d", &coords[i].x, &coords[i].y);
    xs[i] = coords[i].x;
    ys[i] = coords[i].y;
  }

  qsort(xs, NUM_POINTS, sizeof(int), compareInts);
  qsort(ys, NUM_POINTS, sizeof(int), compareInts);

  int prevX = 0, prevY = 0;
  int uIndexX = 0;
  int uIndexY = 0;
  for (int i = 0; i < NUM_POINTS; i++) {
    int currX = xs[i];
    int currY = ys[i];

    if (currX != prevX) {
      uxs[uIndexX] = currX;
      prevX = currX;
      uIndexX++;
    }
    if (currY != prevY) {
      uys[uIndexY] = currY;
      prevY = currY;
      uIndexY++;
    }
  }

  for (int i = 0; i < NUM_POINTS; i++) {
    for (int j = 0; j < uIndexX; j++) {
      if (uxs[j] == coords[i].x) {
        compressed[i].x = j;
        break;
      }
    }
  }

  for (int i = 0; i < NUM_POINTS; i++) {
    for (int j = 0; j < uIndexY; j++) {
      if (uys[j] == coords[i].y) {
        compressed[i].y = j;
        break;
      }
    }
  }

  FILE *f = fopen("output.ppm", "wb");
  fprintf(f, "P6\n");
  int w = 1200, h = 1200;
  int screenBuffer[300][300] = {0};

  for (int i = 0; i < NUM_POINTS; i++) {
    int j = i + 1;
    if (j == NUM_POINTS) {
      j = 0;
    }

    Coord_t currPoint = compressed[i];
    Coord_t nextPoint = compressed[j];
    screenBuffer[currPoint.y][currPoint.x] = 1;
    int adder = 1;
    if (currPoint.x == nextPoint.x) {
      if (currPoint.y > nextPoint.y) {
        adder = -1;
      }
      for (int y = currPoint.y; y != nextPoint.y; y += adder) {
        screenBuffer[y][currPoint.x] = 1;
      }
    } else {
      if (currPoint.x > nextPoint.x) {
        adder = -1;
      }
      for (int x = currPoint.x; x != nextPoint.x; x += adder) {
        screenBuffer[currPoint.y][x] = 1;
      }
    }
  }

  floodFill(screenBuffer, 200, 150);
  fprintf(f, "%d %d\n", w, h);
  fprintf(f, "255\n");
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      fputc(0x00, f); // Red
      if (screenBuffer[y / 4][x / 4]) {
        fputc(0xFF, f); // Green
      } else {
        fputc(0x00, f);
      }
      fputc(0x00, f); // Blue
    }
  }
  fclose(f);

  uint64_t currArea;
  uint64_t maxArea = 0;
  Coord_t maxP1, maxP2;
  for (int i = 0; i < NUM_POINTS; i++) {
    for (int j = i + 1; j < NUM_POINTS; j++) {
      Coord_t p1 = {compressed[i].x, compressed[i].y};
      Coord_t p2 = {compressed[j].x, compressed[j].y};
      currArea =
          (uint64_t)(abs(p1.x - p2.x) + 1) * (uint64_t)(abs(p1.y - p2.y) + 1);
      if (currArea > maxArea) {
        if (isInside(screenBuffer, p1, p2)) {
          maxArea = currArea;
          maxP1.x = coords[i].x;
          maxP1.y = coords[i].y;
          maxP2.x = coords[j].x;
          maxP2.y = coords[j].y;
        }
      }
    }
  }

  maxArea = (uint64_t)(abs(maxP1.x - maxP2.x) + 1) *
            (uint64_t)(abs(maxP1.y - maxP2.y) + 1);

  printf("Max area: %" PRIu64 "\n", maxArea);

  fclose(fp);
}
