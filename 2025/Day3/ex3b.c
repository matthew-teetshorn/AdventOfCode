#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 100
#define NUM_DIGITS 12

int main() {
  char buffer[MAX_LINE_LENGTH + 5];
  char lilBuffer[NUM_DIGITS + 1];
  uint64_t currentJoltage = 0;
  uint64_t totalJoltage = 0;
  char currMax = '0';
  char *endptr; // for strtoull()

  FILE *fp;
  fp = fopen("ex3.input", "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return EXIT_FAILURE;
  }

  while (fgets(buffer, MAX_LINE_LENGTH + 5, fp) != NULL) {
    int p = 0;
    // Search for NUM_DIGITS numbers
    for (int i = 0; i < NUM_DIGITS; i++) {
      // Most Significant Digit (MSD) between previous MSD and EOL
      // with room for remaining digits
      for (int j = p; j <= MAX_LINE_LENGTH - NUM_DIGITS + i; j++) {
        if (buffer[j] > currMax) {
          currMax = buffer[j];
          p = j;
        }
      }
      lilBuffer[i] = currMax;
      currMax = '0';
      ++p;
    }

    currentJoltage = strtoull(lilBuffer, &endptr, 10);
    if (*endptr != '\0') {
      printf("Error: Invalid characters found in lilBuffer\n");
    }

    // printf("Current Joltage: %" PRIu64 "\n", currentJoltage);
    totalJoltage += currentJoltage;
    currentJoltage = 0;
  }

  printf("Total Joltage: %" PRIu64 "\n", totalJoltage);
}
