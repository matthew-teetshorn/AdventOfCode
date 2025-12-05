#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#define DIM 182
#define MAX_LINE 35

int main() {
  FILE *fp;
  char lineBuff[MAX_LINE];
  uint64_t starts[DIM];
  uint64_t ends[DIM];
  uint64_t count = 0;
  int ranges = 0;

  fp = fopen("ex5_ranges.input", "r");

  while (fgets(lineBuff, MAX_LINE, fp) != NULL) {
    sscanf(lineBuff, "%" SCNu64 "-%" SCNu64, &starts[ranges], &ends[ranges]);
    ranges++;
  }

  fclose(fp);
  clock_t start_time = clock();

  // Remove overlaps to avoid double counting
  for (int i = 0; i < ranges; i++) {
    for (int j = 0; j < ranges; j++) {
      // Not overlapping ranges
      if (i == j || ends[i] < starts[j] || starts[i] > ends[j]) {
        continue;
      }
      // Eliminate completely encompassed ranges
      // Then shrink the overlapping ranges
      if (starts[j] >= starts[i] && ends[j] <= ends[i]) {
        starts[j] = 0;
        ends[j] = 0;
      } else if (starts[j] >= starts[i] && starts[j] <= ends[i]) {
        starts[j] = ends[i] + 1;
      } else if (ends[j] <= ends[i] && ends[j] >= starts[i]) {
        ends[j] = starts[i] - 1;
      }
    }
  }

  for (int i = 0; i < ranges; i++) {
    // printf("Range: %" PRIu64 "-%" PRIu64 "\n", starts[i], ends[i]);
    //  Don't count eliminated ranges '0-0'
    if (starts[i] != 0) {
      count += ends[i] - starts[i] + 1;
    }
  }

  clock_t end_time = clock();
  double elapsed = ((double)(end_time - start_time) / CLOCKS_PER_SEC);

  printf("Total fresh ids found: %" PRIu64 "\n", count);
  printf("Total time: %f\n", elapsed);
}
