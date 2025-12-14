#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE  10
#define MIN(a, b) ((a) < (b) ? (a) : (b))

int main() {
  FILE *fp = fopen("ex2.input", "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return EXIT_FAILURE;
  }

  char buffer[MAX_LINE];
  int w, l, h, minside, minperim, area3d, volume3d;
  uint64_t totalPaper  = 0;
  uint64_t totalRibbon = 0;
  while (fgets(buffer, MAX_LINE, fp) != NULL) {
    sscanf(buffer, "%dx%dx%d", &w, &l, &h);
    minside      = MIN(w * h, MIN(w * l, l * h));
    minperim     = MIN(2 * w + 2 * h, MIN(2 * w + 2 * l, 2 * l + 2 * h));
    volume3d     = w * l * h;
    area3d       = (2 * w * h) + (2 * w * l) + (2 * h * l);
    totalPaper  += area3d + minside;
    totalRibbon += volume3d + minperim;
  }

  printf("Total paper needed: %" PRIu64 "\n", totalPaper);
  printf("Total ribbon needed: %" PRIu64 "\n", totalRibbon);
  fclose(fp);
}
