#include <endian.h>
#include <stdio.h>
#include <stdlib.h>

#define PC_WIDTH    3 /* for string operations must us PC_WIDTH + 1 '###\0' */
#define PC_HEIGHT   3
#define NUM_PCS     6
#define MAX_LINE    32
#define NUM_REGIONS 1000

typedef struct {
  int height;
  int width;
  int numPcsByType[NUM_PCS];
} region_t;

typedef struct {
  int squarea; /* actual area of filled squares */
  char squares[PC_HEIGHT][PC_WIDTH + 1];
} piece_t;

void printPiece(piece_t piece) {
  for (int i = 0; i < PC_HEIGHT; i++) {
    printf("%s\n", piece.squares[i]);
  }
  printf("Squarea: %d\n", piece.squarea);
  printf("\n");
}

int main() {
  char buffer[MAX_LINE];
  piece_t pieces[NUM_PCS];
  FILE *fp = fopen("ex12p.input", "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return EXIT_FAILURE;
  }

  // Process Pieces
  while (fgets(buffer, MAX_LINE, fp) != NULL) {
    static int pcIndex = 0;
    static int sqCount = 0;
    static int sqRow   = 0;
    piece_t piece;
    if (buffer[0] != '#' && buffer[0] != '.') continue;
    sscanf(buffer, "%3s", piece.squares[sqRow]);
    for (int i = 0; i < PC_WIDTH; i++) {
      if (piece.squares[sqRow][i] == '#') {
        sqCount++;
      }
    }
    sqRow++;

    if (sqRow == PC_HEIGHT) {
      piece.squarea   = sqCount;
      pieces[pcIndex] = piece;
      sqCount         = 0;
      sqRow           = 0;
      pcIndex++;
    }
  }
  fclose(fp);

  for (int i = 0; i < NUM_PCS; i++) {
    printPiece(pieces[i]);
  }

  // Process regions
  fp = fopen("ex12r.input", "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return EXIT_FAILURE;
  }

  int rIndex = 0;
  region_t regions[NUM_REGIONS];
  region_t region;
  // clang-format off
  while (fscanf(fp, "%2dx%2d: %2d %2d %2d %2d %2d %2d",
                &region.width, &region.height,
                &region.numPcsByType[0], &region.numPcsByType[1],
                &region.numPcsByType[2], &region.numPcsByType[3],
                &region.numPcsByType[4], &region.numPcsByType[5]) != EOF) {
    regions[rIndex++] = region;
  }
  // clang-format on

  // Reduce the search space by ruling out regions that
  // are trivially large (every piece could fit without packing)
  // or impossible small (not enough area for combined squarea)
  int solveable   = 0;
  int unsolveable = 0;
  for (int i = 0; i < NUM_REGIONS; i++) {
    region_t currRegion = regions[i];
    int rArea           = currRegion.width * currRegion.height;
    int rRows           = currRegion.height / PC_HEIGHT;
    int rCols           = currRegion.width / PC_WIDTH;
    int rPcArea         = rRows * rCols;
    int pArea           = 0;
    int pNum            = 0;

    for (int j = 0; j < NUM_PCS; j++) {
      piece_t cPiece  = pieces[j];
      pArea          += currRegion.numPcsByType[j] * cPiece.squarea;
      pNum           += currRegion.numPcsByType[j];
    }

    // All pieces could fit side by side
    if (rPcArea >= pNum) {
      solveable++;
      continue;
    }

    // Pieces could never fit no matter how packed
    if (rArea < pArea) {
      unsolveable++;
    }
  }

  // Apparently all solveable are trivially solveable 😁
  printf("Total   solveable: %d\n", solveable);
  printf("Total unsolveable: %d\n", unsolveable);
  printf("Remaining to be solved: %d\n", NUM_REGIONS - solveable - unsolveable);
}
