#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE        75
#define NUM_INGREDIENTS 4
#define MAX_NAME_LEN    15
#define NUM_TEASPOONS   100

typedef struct {
  int cap;
  int dur;
  int fla;
  int tex;
  int cal;
} totals_t;

typedef struct {
  int cap;
  int dur;
  int fla;
  int tex;
  int cal;
  totals_t tots;
  char name[MAX_NAME_LEN];
} ingredient_t;

void sumIngredient(ingredient_t *i, int n) {
  i->tots.cap = n * i->cap;
  i->tots.dur = n * i->dur;
  i->tots.fla = n * i->fla;
  i->tots.tex = n * i->tex;
  i->tots.cal = n * i->cal;
}

int main() {
  FILE *fp = fopen("ex15.input", "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return EXIT_FAILURE;
  }

  char buffer[MAX_LINE];
  ingredient_t is[NUM_INGREDIENTS];
  ingredient_t *pi = is;
  while (fgets(buffer, MAX_LINE, fp) != NULL) {
    // clang-format off
    sscanf(
      buffer,
      "%[^:]: capacity %d, durability %d, flavor %d, texture %d, calories %d",
      pi->name, &pi->cap, &pi->dur, &pi->fla, &pi->tex, &pi->cal);
    // clang-format on
    pi++;
  }
  fclose(fp);

  uint64_t part1Score = 0, part2Score = 0;
  uint64_t currScore;
  for (int i = 0; i <= NUM_TEASPOONS; i++) {
    for (int j = 0; j <= NUM_TEASPOONS - i; j++) {
      for (int k = 0; k <= NUM_TEASPOONS - i - j; k++) {
        int l = NUM_TEASPOONS - i - j - k;

        sumIngredient(&is[0], i);
        sumIngredient(&is[1], j);
        sumIngredient(&is[2], k);
        sumIngredient(&is[3], l);

        int capScore =
            is[0].tots.cap + is[1].tots.cap + is[2].tots.cap + is[3].tots.cap;
        int durScore =
            is[0].tots.dur + is[1].tots.dur + is[2].tots.dur + is[3].tots.dur;
        int flaScore =
            is[0].tots.fla + is[1].tots.fla + is[2].tots.fla + is[3].tots.fla;
        int texScore =
            is[0].tots.tex + is[1].tots.tex + is[2].tots.tex + is[3].tots.tex;
        int calTotal =
            is[0].tots.cal + is[1].tots.cal + is[2].tots.cal + is[3].tots.cal;

        if (capScore < 0 || durScore < 0 || flaScore < 0 || texScore < 0) {
          currScore = 0;
        } else {
          currScore = (uint64_t)capScore * durScore * flaScore * texScore;
        }

        if (currScore > part1Score) {
          part1Score = currScore;
        }
        if (currScore > part2Score && calTotal == 500) {
          part2Score = currScore;
        }
      }
    }
  }

  printf("Best cookie score found for Part1: %" PRIu64 "\n", part1Score);
  printf("Best cookie score found for Part2: %" PRIu64 "\n", part2Score);
}
