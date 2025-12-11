#include <iso646.h>
#include <search.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NEL 546
#define ID_LEN 3
#define MAX_OUT 20
#define MAX_LINE 100

typedef struct {
  int pathsToEnd;
  int numOutputs;
  char outputs[MAX_OUT][ID_LEN + 1];
} link_t;

link_t *createLink() {
  link_t *newLink = malloc(sizeof(link_t));
  if (newLink == NULL) {
    printf("Error allocating link\n");
    return NULL;
  }
  newLink->numOutputs = 0;
  newLink->pathsToEnd = -1;
  return newLink;
}

ENTRY createEntry(int *success) {
  ENTRY e;
  e.key = malloc(ID_LEN + 1);
  e.data = malloc(sizeof(link_t));
  if (e.data == NULL) {
    printf("Error creating ENTRY\n");
    *success = 0;
    return e;
  }
  *success = 1;
  return e;
}

int countPaths(ENTRY *ep) {
  char *currKey = ep->key;
  link_t *currLink = (link_t *)ep->data;
  int pathsToEnd = 0;

  // found end point
  if (!strcmp(currLink->outputs[0], "out")) {
    return 1;
  }

  // If we have already calculated paths from here, return
  if (currLink->pathsToEnd != -1) {
    return currLink->pathsToEnd;
  }

  // DFS possible paths
  for (int i = 0; i < currLink->numOutputs; i++) {
    ENTRY *next =
        hsearch((ENTRY){.key = currLink->outputs[i], .data = NULL}, FIND);
    if (next == NULL) {
      printf("Failed to find next node\n");
      return 0;
    }
    pathsToEnd += countPaths(next);
  }

  // Memoize the computed path
  currLink->pathsToEnd = pathsToEnd;
  return pathsToEnd;
}

int main() {
  FILE *fp = fopen("ex11.input", "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return EXIT_FAILURE;
  }

  // Using hash map for lookups
  hcreate((size_t)NEL);
  int numHashEntries = 0;
  // Need to track all entries malloc'd for later free
  ENTRY *hashEntries[NEL] = {0};

  char buffer[MAX_LINE];
  char test[NEL][ID_LEN + 1];
  int entryIndex = 0;
  ENTRY e;
  while (fgets(buffer, MAX_LINE, fp) != NULL) {
    char *bufPtr = buffer;
    int index = 0;
    link_t *l = createLink();

    int success = 1;
    e = createEntry(&success);
    if (!success) {
      printf("Error creating hash entry\n");
      return EXIT_FAILURE;
    }
    hashEntries[entryIndex++] = &e;
    e.data = l;

    sscanf(bufPtr, "%3s:%n", e.key, &index);
    for (int i = 0; sscanf(bufPtr += index, "%3s%n", l->outputs[i], &index) > 0;
         i++, l->numOutputs++);
    hsearch(e, ENTER);
  }

  clock_t start_time = clock();
  ENTRY *start = hsearch((ENTRY){.key = "you", .data = NULL}, FIND);

  // Search the hashmap via DFS and count paths
  int totalPaths = countPaths(start);
  clock_t end_time = clock();
  double elapsed = (double)(end_time - start_time) / CLOCKS_PER_SEC;

  printf("Total Paths: %d\n", totalPaths);
  printf("Elapsed time: %f\n", elapsed);

  // Cleanup
  hdestroy();
  fclose(fp);
}
