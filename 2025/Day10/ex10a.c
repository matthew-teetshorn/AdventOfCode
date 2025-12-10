#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

#define DIM 212
#define MAX_JOLT 10
#define MAX_BUTTONS 13
#define MAX_SUBSETS 8192

typedef struct {
  int numElements;
  int members[MAX_BUTTONS];
} Set;

int compSets(const void *a, const void *b) {
  Set setA = *(const Set *)a;
  Set setB = *(const Set *)b;
  if (setA.numElements < setB.numElements) {
    return -1;
  } else if (setA.numElements > setB.numElements) {
    return 1;
  }
  return 0;
}

// Generate a sorted subset array with single sets first
void calcSubsets(Set buffer[MAX_SUBSETS], int n) {
  Set newSet;
  int memIdx = 0;
  newSet.numElements = 0;
  for (int i = 1; i < (1 << n); i++) {
    for (int j = 0; j < n; j++) {
      if ((i & (1 << j)) != 0) {
        newSet.numElements++;
        newSet.members[memIdx] = j;
        memIdx++;
      }
    }
    buffer[i - 1] = newSet;
    newSet.numElements = 0;
    memIdx = 0;
  }
  qsort(buffer, (1 << n) - 1, sizeof(Set), compSets);
}

int main() {
  FILE *fp = fopen("ex10.input", "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return EXIT_FAILURE;
  }

  char buffer[DIM + 2];
  uint16_t lights = 0;
  uint16_t buttons[MAX_BUTTONS] = {0};
  uint16_t joltages[MAX_JOLT] = {0};
  uint64_t total = 0;
  int numButtons = 0;
  int numJoltages = 1;
  int maxNumButtons = 0;
  Set subsets[MAX_SUBSETS];

  while (fgets(buffer, DIM + 2, fp) != NULL) {
    int numLights = 0;
    int numSteps = 0;

    // read lights
    char *bufPtr = buffer + 1;

    while (*bufPtr != '\(') {
      switch (*bufPtr) {
      case '.':
        lights <<= 1;
        numLights++;
        break;
      case '#':
        lights = (lights << 1) | 1;
        numLights++;
        break;
      }
      bufPtr++;
    }

    // read buttons
    int bIdx = 0;
    while (*bufPtr != '{') {
      if (*bufPtr == '\(') {
        numButtons++;
        bufPtr++;
        buttons[bIdx] = 0;
        for (int i = 0; i < numLights; i++) {
          if (*bufPtr != i + 48) {
            buttons[bIdx] <<= 1;
            continue;
          }
          buttons[bIdx] = (buttons[bIdx] << 1) | 1;
          bufPtr += 2;
        }
        bIdx++;
      }
      bufPtr++;
    }

    // read joltages
    int joltIdx = 0;
    joltages[joltIdx] = 0;
    while (*(++bufPtr) != '}') {
      if (*bufPtr == ',') {
        numJoltages++;
        joltIdx++;
        joltages[joltIdx] = 0;
        continue;
      }
      joltages[joltIdx] = joltages[joltIdx] * 10 + (*bufPtr - '0');
    }

    // Process button presses
    uint64_t numPresses = 0;
    calcSubsets(subsets, numButtons);
    for (int i = 0; i < (1 << numButtons); i++) {
      uint16_t pressed = 0;
      for (numPresses = 0; numPresses < subsets[i].numElements; numPresses++) {
        uint16_t currButton = buttons[subsets[i].members[numPresses]];
        pressed ^= currButton;
      }
      if (pressed == lights) {
        total += numPresses;
        break;
      }
    }

    numButtons = 0;
    numJoltages = 1;
    lights = 0;
  }
  printf("Total button presses: %" PRIu64 "\n", total);

  fclose(fp);
}
