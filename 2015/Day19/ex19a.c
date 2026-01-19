#include <ctype.h>
#include <search.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE       20
#define MAX_TRANSFORMS 13
#define MAX_TRANS_PER  10
#define MAX_TRANS_LEN  11
#define MAX_ATOM       3
#define MAX_MOLECULE   1024
#define MAX_UNIQUE     1024

typedef struct {
  int count;
  char atom[MAX_ATOM];
  char transforms[MAX_TRANS_PER][MAX_TRANS_LEN];
} replacement_t;

int main() {
  FILE *fp = fopen("ex19.input", "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return EXIT_FAILURE;
  }

  replacement_t transforms[MAX_TRANSFORMS] = {0};
  int num_transforms                       = 0;

  char buffer[MAX_LINE];
  char atom_buffer[MAX_ATOM];
  char trans_buffer[MAX_TRANS_LEN];
  while (fgets(buffer, MAX_LINE, fp) != NULL) {
    sscanf(buffer, "%s => %s", atom_buffer, trans_buffer);

    int found = 0;
    for (int i = 0; i < num_transforms; i++) {
      if (!strcmp(atom_buffer, transforms[i].atom)) { // Atom seen
        replacement_t *t = &transforms[i];
        strcpy(t->transforms[t->count++], trans_buffer);
        found = 1;
        break;
      }
    }

    if (!found) {
      replacement_t *t = &transforms[num_transforms++];
      strcpy(t->atom, atom_buffer);
      strcpy(t->transforms[t->count++], trans_buffer);
    }
  }
  fclose(fp);

  char molecule[MAX_MOLECULE];
  fp = fopen("ex19mol.input", "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return EXIT_FAILURE;
  }
  fgets(molecule, MAX_MOLECULE, fp);
  molecule[strlen(molecule) - 1] = 0; // strip '\n'
  fclose(fp);

  // Need a hash table for unique molecules
  hcreate(1024);
  ENTRY e, *ep;
  char uniqueMolecules[MAX_UNIQUE][MAX_MOLECULE];

  int step                       = 0;
  int numReplacements            = 0;
  int numAtoms                   = 0;
  int uniqueIndex                = 0;
  char newMolecule[MAX_MOLECULE] = {0};
  int molecule_length            = strlen(molecule);
  for (char *p = molecule; p - molecule < molecule_length;) {
    numAtoms++;
    int curr_transform = 0;
    char curr_atom[MAX_ATOM];
    // Get len of current atomic symbol
    if (islower(*(p + 1))) step = 2;
    else step = 1;
    strncpy(curr_atom, p, step);
    curr_atom[step] = 0;

    int hasTransform = 0;
    for (int i = 0; i < num_transforms; i++) {
      if (!strcmp(transforms[i].atom, curr_atom)) {
        curr_transform = i;
        hasTransform   = 1;
        break;
      }
    }

    if (!hasTransform) {
      p += step;
      continue;
    }

    replacement_t *current = &transforms[curr_transform];
    for (int i = 0; i < current->count; i++) {
      numReplacements++;
      // Prepend previous atoms
      int toAppend = p - molecule;
      strncpy(newMolecule, molecule, toAppend);
      newMolecule[toAppend] = 0;
      // Make the atom replacement
      strcat(newMolecule, current->transforms[i]);
      // Append the rest
      strcat(newMolecule, p + step);

      // See if we've encoutered this molecule before
      e.key  = newMolecule;
      e.data = NULL;
      ep     = hsearch(e, FIND);
      if (ep == NULL) {
        // Place in the unique store
        strcpy(uniqueMolecules[uniqueIndex], newMolecule);
        e.key = uniqueMolecules[uniqueIndex++];
        hsearch(e, ENTER);
      }
    }
    p += step;
  }
  printf("Number of unique molecules found: %d\n", uniqueIndex);
  hdestroy();
}
