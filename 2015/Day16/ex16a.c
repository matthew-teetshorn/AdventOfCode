#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 75

typedef struct {
  int children;
  int cats;
  int samoyeds;
  int pomeranians;
  int akitas;
  int vizslas;
  int goldfish;
  int trees;
  int cars;
  int perfumes;
} aunt_t;

int *resolveMember(aunt_t *aunt, char *buffer) {
  if (!strcmp(buffer, "children")) {
    return &aunt->children;
  } else if (!strcmp(buffer, "cats")) {
    return &aunt->cats;
  } else if (!strcmp(buffer, "samoyeds")) {
    return &aunt->samoyeds;
  } else if (!strcmp(buffer, "pomeranians")) {
    return &aunt->pomeranians;
  } else if (!strcmp(buffer, "akitas")) {
    return &aunt->akitas;
  } else if (!strcmp(buffer, "vizslas")) {
    return &aunt->vizslas;
  } else if (!strcmp(buffer, "goldfish")) {
    return &aunt->goldfish;
  } else if (!strcmp(buffer, "trees")) {
    return &aunt->trees;
  } else if (!strcmp(buffer, "cars")) {
    return &aunt->cars;
  } else if (!strcmp(buffer, "perfumes")) {
    return &aunt->perfumes;
  }
  // Shouldn't happen
  return NULL;
}

int compSues(aunt_t mfcsam, aunt_t other) {
  int *mPtr = &mfcsam.children;
  int *oPtr = &other.children;

  for (int i = 0; i < 10; i++) {
    if (*oPtr == -1) { // other value unknown
      oPtr++;
      mPtr++;
    } else if (*mPtr++ != *oPtr++) { // compare known values
      return 0;
    }
  }
  return 1;
}

int main() {
  FILE *fp = fopen("ex16.input", "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return EXIT_FAILURE;
  }

  aunt_t MFCSAM = {
      .children    = 3,
      .cats        = 7,
      .samoyeds    = 2,
      .pomeranians = 3,
      .akitas      = 0,
      .vizslas     = 0,
      .goldfish    = 5,
      .trees       = 3,
      .cars        = 2,
      .perfumes    = 1,
  };

  char buffer[MAX_LINE];
  char varBuffers[3][12];
  int intBuffers[3];
  int sueIndex;
  while (fgets(buffer, MAX_LINE, fp) != NULL) {
    aunt_t currSue = {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    };
    sscanf(buffer, "Sue %d: %[^:]: %d, %[^:]: %d, %[^:]: %d", &sueIndex,
           varBuffers[0], &intBuffers[0], varBuffers[1], &intBuffers[1],
           varBuffers[2], &intBuffers[2]);

    *resolveMember(&currSue, varBuffers[0]) = intBuffers[0];
    *resolveMember(&currSue, varBuffers[1]) = intBuffers[1];
    *resolveMember(&currSue, varBuffers[2]) = intBuffers[2];

    if (compSues(MFCSAM, currSue)) {
      printf("Matching Sue: %d\n", sueIndex);
      break;
    }
  }
  fclose(fp);
}
