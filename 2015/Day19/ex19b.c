#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MOLECULE 1024

int main() {
  char molecule[MAX_MOLECULE];
  FILE *fp = fopen("ex19mol2.input", "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return EXIT_FAILURE;
  }
  fgets(molecule, MAX_MOLECULE, fp);
  molecule[strlen(molecule) - 1] = 0; // strip '\n'
  fclose(fp);

  char *p      = molecule;
  int length   = strlen(molecule);
  int elements = 0;
  int parens   = 0;
  int commas   = 0;
  while (p - molecule < length) {
    elements++;
    // Input molecule has been modified as follows:
    // 'Rn' => '(', 'Ar' => ')', 'Y' => ','
    if (*p == '(' || *p == ')') {
      parens++;
    } else if (*p == ',') {
      commas++;
    } else if (p - molecule < length - 1 && islower(*(p + 1))) {
      p++;
    }
    p++;
  }

  // Analysis of the molecule demonstrated a mathematical
  // formulation for determining total replacements
  int final = elements - parens - (2 * commas) - 1;
  printf("e: %d p: %d c: %d\n", elements, parens, commas);
  printf("Final: %d\n", final);
}
