#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE       20
#define MAX_TRANS_PER  10
#define MAX_LEN_MOL    11
#define MAX_LEN_ATOM   3
#define NUM_TRANSFORMS 43
#define MAX_MOLECULE   1024
#define MAX_UNIQUE     1024

typedef struct {
  char molecule[MAX_LEN_MOL];
  char atom[MAX_LEN_ATOM];
} replacement_t;

int comp_repls(const void *a, const void *b) {
  char *pa = (char *)a;
  char *pb = (char *)b;

  return (strlen(pa) - strlen(pb));
}

int main() {
  FILE *fp = fopen("ex19.input", "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return EXIT_FAILURE;
  }

  replacement_t transforms[NUM_TRANSFORMS] = {0};
  int num_transforms                       = 0;

  char buffer[MAX_LINE];
  while (fgets(buffer, MAX_LINE, fp) != NULL) {
    sscanf(buffer, "%s => %s", transforms[num_transforms].atom,
           transforms[num_transforms].molecule);
    num_transforms++;
  }
  fclose(fp);

  qsort(transforms, num_transforms, sizeof(replacement_t), comp_repls);

  char molecule[MAX_MOLECULE];
  fp = fopen("ex19mol.input", "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return EXIT_FAILURE;
  }
  fgets(molecule, MAX_MOLECULE, fp);
  molecule[strlen(molecule) - 1] = 0; // strip '\n'
  fclose(fp);

  char *goal                       = "e";
  char *p                          = molecule;
  char next_molecule[MAX_MOLECULE] = {0};
  int replaced                     = 0;
  int len_replaced                 = 0;

  // while (strcmp(goal, molecule)) {
  //   printf("%s\n", molecule);
  //
  //   replaced        = 0;
  //   int curr_length = strlen(molecule);
  //   while (p - molecule < curr_length) {
  //     // while (!replaced) {
  //     for (int i = 0; i < num_transforms; i++) {
  //       len_replaced = strlen(transforms[i].molecule);
  //       if (!strncmp(p, transforms[i].molecule, len_replaced)) {
  //         strcat(next_molecule, transforms[i].atom);
  //         printf("\nReplacing: %s with: %s\n", transforms[i].molecule,
  //                transforms[i].atom);
  //         replaced = 1;
  //         break;
  //       }
  //     }
  //     if (replaced) {
  //       p += len_replaced;
  //     } else {
  //       strncat(next_molecule, p, 1);
  //       p++;
  //       if (p - molecule >= strlen(molecule)) replaced = 1;
  //     }
  //   }
  //
  //   strcpy(molecule, next_molecule);
  //   next_molecule[0] = '\0';
  //   p                = molecule;
  // }

  printf("%s\n", molecule);
}
