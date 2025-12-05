#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

#define DIM 182
#define MAX_LINE 35

int main() {
  FILE *fp;
  char lineBuff[MAX_LINE];
  uint64_t starts[DIM];
  uint64_t ends[DIM];
  uint64_t current = 0;
  int ranges = 0;
  int count = 0;

  fp = fopen("ex5_ranges.input", "r");

  while (fgets(lineBuff, MAX_LINE, fp) != NULL) {
    sscanf(lineBuff, "%" SCNu64 "-%" SCNu64, &starts[ranges], &ends[ranges]);
    ranges++;
  }

  fclose(fp);
  fp = fopen("ex5_ids.input", "r");

  while (fscanf(fp, "%" SCNu64, &current) != EOF) {
    for (int i = 0; i < ranges; i++) {
      // printf("Ranges: %" PRIu64 "-%" PRIu64 "\tCurrent: %" PRIu64 "\n",
      //        starts[i], ends[i], current);
      if (current >= starts[i] && current <= ends[i]) {
        count++;
        break;
      }
    }
  }

  printf("Total fresh items found: %d\n", count);

  fclose(fp);
}
