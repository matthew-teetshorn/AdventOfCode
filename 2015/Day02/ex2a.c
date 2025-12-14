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
  int w, l, h, minside, area3d;
  uint64_t total = 0;
  while (fgets(buffer, MAX_LINE, fp) != NULL) {
    sscanf(buffer, "%dx%dx%d", &w, &l, &h);
    minside  = MIN(w * h, MIN(w * l, l * h));
    area3d   = (2 * w * h) + (2 * w * l) + (2 * h * l);
    total   += area3d + minside;
  }

  printf("Total area needed: %" PRIu64 "\n", total);
  fclose(fp);
}
