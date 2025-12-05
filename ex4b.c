#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DIM 135

int countNeighbors(int arr[DIM][DIM], int r, int c) {
  int count = 0;

  for (int dR = -1; dR <= 1; dR++) {
    for (int dC = -1; dC <= 1; dC++) {
      int nR = r + dR;
      int nC = c + dC;

      if (nR == r && nC == c) {
        continue;
      }

      if (nR >= 0 && nR < DIM && nC >= 0 && nC < DIM) {
        if (arr[nR][nC]) {
          count++;
        }
      }
    }
  }

  return count;
}

int main() {
  FILE *fp;
  int arr[DIM][DIM];
  char c;
  int row = 0;
  int col = 0;
  int rollCount = 0;
  int pRollCount = 1;

  fp = fopen("ex4.input", "r");
  if (fp == NULL) {
    printf("Error opening input file");
    return EXIT_FAILURE;
  }

  while ((c = fgetc(fp)) != EOF) {
    if (c != '.' && c != '@') {
      continue;
    } else if (c == '.') {
      arr[row][col++] = 0;
    } else {
      arr[row][col++] = 1;
    }
    if (col == DIM) {
      col = 0;
      row++;
    }
  }

  clock_t start_time = clock();

  while (1) {
    for (int r = 0; r < DIM; r++) {
      for (int c = 0; c < DIM; c++) {
        if (arr[r][c]) {
          int count = countNeighbors(arr, r, c);
          if (count < 4) {
            rollCount++;
            arr[r][c] = 0;
          }
        }
      }
    }
    if (rollCount == pRollCount) {
      break;
    }
    pRollCount = rollCount;
  }

  printf("Roll Count: %d\n", rollCount);

  clock_t end_time = clock();
  double elapsed = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
  printf("Elapsed time: %f\n", elapsed);
}
