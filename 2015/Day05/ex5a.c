#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 20

int main() {
  FILE *fp = fopen("ex5.input", "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return EXIT_FAILURE;
  }

  char buffer[MAX_LINE];
  char *first, *second;
  int vowelsNeeded = 3;
  int totalGood    = 0;
  int totalBad     = 0;
  char *bad1       = "ab";
  char *bad2       = "cd";
  char *bad3       = "pq";
  char *bad4       = "xy";

  while (fgets(buffer, MAX_LINE, fp) != NULL) {
    int vowelCount = 0;
    bool hasDouble = false;
    bool isBad     = false;
    first          = buffer;
    second         = buffer + 1;
    for (; *second != '\0'; ++first, ++second) {
      // clang-format off
      if (*first == 'a' || *first == 'e' ||
          *first == 'i' || *first == 'o' ||
          *first == 'u') {
        vowelCount++;
      }
      // clang-format on
      if (*first == *second) hasDouble = true;
      if (!strncmp(first, bad1, 2)) isBad = true;
      if (!strncmp(first, bad2, 2)) isBad = true;
      if (!strncmp(first, bad3, 2)) isBad = true;
      if (!strncmp(first, bad4, 2)) isBad = true;
    }

    if (hasDouble && vowelCount >= vowelsNeeded && !isBad) {
      totalGood++;
    } else {
      totalBad++;
    }
  }

  printf("Total good strings: %d\n", totalGood);
  printf("Total bad strings: %d\n", totalBad);
  fclose(fp);
}
