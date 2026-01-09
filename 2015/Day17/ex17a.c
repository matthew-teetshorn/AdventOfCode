#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE       4
#define NUM_CONTAINERS 20
#define TARGET         150

typedef struct Combination {
  int index;
  int containers[NUM_CONTAINERS];
  struct Combination *next;
} combination_t;

typedef struct {
  combination_t *head;
  combination_t *tail;
} combination_list_t;

bool addValueToCombination(combination_t *c, int v) {
  if (c->index >= NUM_CONTAINERS) return false;
  c->containers[c->index++] = v;
  return true;
}
combination_t *addCombinationToList(combination_list_t *cList,
                                    combination_t *combination) {
  if (cList == NULL) return NULL;

  combination_t *new = malloc(sizeof(combination_t));
  if (new == NULL) return NULL;
  new->next = NULL;

  for (int i = 0; i < NUM_CONTAINERS; i++) {
    if (!addValueToCombination(new, combination->containers[i])) {
      printf("Error adding value to container [ACTL]: container full\n");
    }
  }

  if (cList->head == NULL) {
    cList->head = cList->tail = new;
  } else {
    cList->tail->next = new;
    cList->tail       = new;
  }

  return new;
}

void genCombinations(int *remainingContainers,
                     combination_t *currentCombination,
                     combination_list_t *cList, int length, int value) {
  printf("Looking at: %d with value remaining: %d\n", *remainingContainers,
         value);
  // Base case
  if (value == 0) {
    combination_t *c = addCombinationToList(cList, currentCombination);
    if (c == NULL) {
      printf("Error adding combination to list\n");
    }
    return;
  }

  // Find next small enough container
  for (int i = 0; i < length; i++) {
    int currContainer = remainingContainers[i];
    if (currContainer <= value) {
      int currIndex = currentCombination->index;
      if (addValueToCombination(currentCombination, currContainer)) {
        genCombinations(&remainingContainers[i + 1], currentCombination, cList,
                        length - i - 1, value - currContainer);
        // Reset the index (pop added values)
        currentCombination->index = currIndex;
      } else {
        printf("Error adding value to container [GC]: container full\n");
      }
    }
  }
}

void listFree(combination_list_t *cList) {
  if (cList == NULL) return;
  combination_t *curr, *next;
  curr = cList->head;
  while (curr != NULL) {
    next = curr->next;
    free(curr);
    curr = next;
  }
}

int countCombinations(combination_list_t *cList) {
  int retVal          = 0;
  combination_t *curr = cList->head;
  while (curr != NULL) {
    retVal++;
    curr = curr->next;
  }
  return retVal;
}

void printCombinations(combination_list_t *cList) {
  combination_t *curr;
  curr = cList->head;
  while (curr != NULL) {
    printf("[");

    for (int i = 0; i < curr->index; i++) {
      printf("%2d", curr->containers[i]);

      if (i < curr->index - 1) {
        printf(", ");
      }
    }
    printf("]\n");
    curr = curr->next;
  }
}

int main() {
  FILE *fp = fopen("ex17.input", "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return EXIT_FAILURE;
  }

  char buffer[MAX_LINE];
  int containers[NUM_CONTAINERS];
  int cIndex = 0;
  while (fgets(buffer, MAX_LINE, fp) != NULL) {
    sscanf(buffer, "%d", &containers[cIndex++]);
  }
  fclose(fp);

  combination_list_t cList;
  cList.head = cList.tail          = NULL;
  combination_t currentCombination = {
      .index = 0, .containers = {0}, .next = NULL};
  genCombinations(containers, &currentCombination, &cList, NUM_CONTAINERS,
                  TARGET);

  int numCombinations = countCombinations(&cList);
  printCombinations(&cList);
  printf("Num combinations found: %d\n", numCombinations);

  listFree(&cList);
}
