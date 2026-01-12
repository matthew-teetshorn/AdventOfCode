#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE       102
#define GRID_SIZE      100
#define GENERATIONS    100
#define NANO_TO_MILLIS 1000000
#define PART2
// #define ANIMATE

#ifdef ANIMATE
#include <time.h>
#endif

void swapBuffers(int (**b1)[GRID_SIZE], int (**b2)[GRID_SIZE]) {
  int (*temp)[GRID_SIZE] = *b1;
  *b1                    = *b2;
  *b2                    = temp;
}

int calcNextGen(int grid[GRID_SIZE][GRID_SIZE], int row, int col) {
  int neighbors = 0;
  int nextRow, nextCol;
  int isAlive = grid[row][col];

  for (int dx = -1; dx <= 1; dx++) {
    for (int dy = -1; dy <= 1; dy++) {
      nextRow = row + dy;
      nextCol = col + dx;
      if (nextRow < 0 || nextRow >= GRID_SIZE || nextCol < 0 ||
          nextCol >= GRID_SIZE || (dx == 0 && dy == 0)) {
        continue;
      }
      neighbors += grid[nextRow][nextCol];
    }
  }

  if ((isAlive && (neighbors == 2 || neighbors == 3)) ||
      (!isAlive && neighbors == 3)) {
    return 1;
  }
  return 0;
}

int countOn(int grid[GRID_SIZE][GRID_SIZE]) {
  int count = 0;

  for (int r = 0; r < GRID_SIZE; r++) {
    for (int c = 0; c < GRID_SIZE; c++) {
      count += grid[r][c];
    }
  }

  return count;
}

void printGrid(int grid[GRID_SIZE][GRID_SIZE]) {
  // Clear screen and return
  printf("\033[2J\033[1;1H");

  for (int r = 0; r < GRID_SIZE; r++) {
    for (int c = 0; c < GRID_SIZE; c++) {
      printf("%d", grid[r][c]);
    }
    printf("\n");
  }
}

int main() {
  FILE *fp = fopen("ex18.input", "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return EXIT_FAILURE;
  }

  char buffer[MAX_LINE];
  int gridOne[GRID_SIZE][GRID_SIZE];
  int gridTwo[GRID_SIZE][GRID_SIZE] = {0};
  int row                           = 0;
  while (fgets(buffer, MAX_LINE, fp) != NULL) {
    for (int col = 0; col < GRID_SIZE; col++) {
      if (buffer[col] == '#') {
        gridOne[row][col] = 1;
      } else gridOne[row][col] = 0;
    }
    row++;
  }
  fclose(fp);

  int (*curr)[GRID_SIZE] = gridOne, (*next)[GRID_SIZE] = gridTwo;
#ifdef ANIMATE
  struct timespec wait = {.tv_sec = 0, .tv_nsec = 100 * NANO_TO_MILLIS};
#endif

  for (int gen = 0; gen < GENERATIONS; gen++) {
    for (int row = 0; row < GRID_SIZE; row++) {
      for (int col = 0; col < GRID_SIZE; col++) {
        next[row][col] = calcNextGen(curr, row, col);
      }
    }
#ifdef ANIMATE
    printGrid(next);
    nanosleep(&wait, NULL);
#endif
#ifdef PART2
    // Corners always on in Part 2
    next[0][0] = next[0][GRID_SIZE - 1]    = next[GRID_SIZE - 1][0] =
        next[GRID_SIZE - 1][GRID_SIZE - 1] = 1;
#endif
    swapBuffers(&curr, &next);
  }

  int total = countOn(curr);
  printf("Total lights on after %d generations: %d\n", GENERATIONS, total);
}
