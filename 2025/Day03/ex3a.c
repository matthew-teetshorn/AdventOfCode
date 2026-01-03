#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 200

int main() {
  char buffer[MAX_LINE_LENGTH];
  char lilBuffer[3];
  char *fMaxPtr, *sMaxPtr;
  char firstMax, secondMax;
  int currentJoltage = 0;
  int totalJoltage   = 0;

  FILE *fp;
  fp = fopen("ex3.input", "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return EXIT_FAILURE;
  }

  while (fgets(buffer, MAX_LINE_LENGTH, fp) != NULL) {
    firstMax  = buffer[0];
    secondMax = '0';
    fMaxPtr   = buffer;
    sMaxPtr   = fMaxPtr + 1;

    while (*sMaxPtr != '\n') {
      if (*sMaxPtr > *fMaxPtr && *(sMaxPtr + 1) != '\n') {
        fMaxPtr   = sMaxPtr;
        firstMax  = *fMaxPtr;
        secondMax = '0';
      } else if (*sMaxPtr > secondMax) {
        secondMax = *sMaxPtr;
      }
      sMaxPtr++;
    }

    sprintf(lilBuffer, "%c%c", firstMax, secondMax);
    // sscanf(lilBuffer, "%d", &currentJoltage);
    currentJoltage = atoi(lilBuffer);
    printf("%d\n", currentJoltage);
    totalJoltage += currentJoltage;
  }

  printf("Total Joltage: %d\n", totalJoltage);
  fclose(fp);
}
