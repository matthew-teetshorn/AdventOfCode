#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 75

int resolveMember(char *buffer) {
  if (!strcmp(buffer, "children")) {
    return 0;
  } else if (!strcmp(buffer, "cats")) {
    return 1;
  } else if (!strcmp(buffer, "samoyeds")) {
    return 2;
  } else if (!strcmp(buffer, "pomeranians")) {
    return 3;
  } else if (!strcmp(buffer, "akitas")) {
    return 4;
  } else if (!strcmp(buffer, "vizslas")) {
    return 5;
  } else if (!strcmp(buffer, "goldfish")) {
    return 6;
  } else if (!strcmp(buffer, "trees")) {
    return 7;
  } else if (!strcmp(buffer, "cars")) {
    return 8;
  } else if (!strcmp(buffer, "perfumes")) {
    return 9;
  }
  // Shouldn't happen
  return -1;
}

int compSues(int mfcsam[], int other[]) {
  for (int i = 0; i < 10; i++) {
    if (other[i] == -1) continue;
    if (i == 1 || i == 7) { // cats && trees
      if (other[i] <= mfcsam[i]) return 0;
    } else if (i == 3 || i == 6) { // pomeranians && goldfish
      if (other[i] >= mfcsam[i]) return 0;
    } else if (other[i] != mfcsam[i]) return 0;
  }
  return 1;
}

int main() {
  FILE *fp = fopen("ex16.input", "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return EXIT_FAILURE;
  }

  int MFCSAM[] = {3, 7, 2, 3, 0, 0, 5, 3, 2, 1};

  char buffer[MAX_LINE];
  char varBuffers[3][12];
  int intBuffers[3];
  int sueIndex;
  while (fgets(buffer, MAX_LINE, fp) != NULL) {
    int currSue[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    sscanf(buffer, "Sue %d: %[^:]: %d, %[^:]: %d, %[^:]: %d", &sueIndex,
           varBuffers[0], &intBuffers[0], varBuffers[1], &intBuffers[1],
           varBuffers[2], &intBuffers[2]);

    currSue[resolveMember(varBuffers[0])] = intBuffers[0];
    currSue[resolveMember(varBuffers[1])] = intBuffers[1];
    currSue[resolveMember(varBuffers[2])] = intBuffers[2];

    if (compSues(MFCSAM, currSue)) {
      printf("Matching Sue: %d\n", sueIndex);
      break;
    }
  }
  fclose(fp);
}
