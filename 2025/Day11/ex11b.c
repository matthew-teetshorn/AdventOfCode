#include <inttypes.h>
#include <iso646.h>
#include <search.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

#define MAX_KEYS 4096
#define NEL      546
#define MAX_LINE 100
#define MAX_OUT  20
#define ID_LEN   4 /* 'CCC\0' */
#define KEY_LEN  6 /* 'CCCNN\0' */

char nodeOutputs[NEL][MAX_OUT][ID_LEN] = {0};
char nodes[NEL][ID_LEN]                = {0};
int numOutputs[NEL]                    = {0};

ENTRY createEntry() {
  // Static storage for allocating keys and data in the hash table
  static uint64_t dataStore[MAX_KEYS][sizeof(uint64_t)];
  static char keyStore[MAX_KEYS][KEY_LEN];
  static int ksIDX = 0;
  ENTRY e;
  e.key  = keyStore[ksIDX];
  e.data = dataStore[ksIDX++];
  return e;
}

uint64_t countPaths(int startIndex, int fftSeen, int dacSeen) {
  // Store function parameters as hashmap entries
  ENTRY e = createEntry();
  snprintf(e.key, KEY_LEN, "%3s%1d%1d", nodes[startIndex], fftSeen, dacSeen);
  uint64_t *retval = e.data;

  ENTRY *ep = hsearch(e, FIND);
  if (ep != NULL) {
    // Been here before
    return *(uint64_t *)(ep->data);
  }

  if (!strcmp(nodes[startIndex], "fft")) {
    fftSeen = 1;
  }
  if (!strcmp(nodes[startIndex], "dac")) {
    dacSeen = 1;
  }

  if (!strcmp(nodeOutputs[startIndex][0], "out")) {
    if (fftSeen && dacSeen) {
      *retval = 1;
    } else {
      *retval = 0;
    }
    return *retval;
  }

  for (int i = 0; i < numOutputs[startIndex]; i++) {
    for (int j = 0; j < NEL; j++) {
      if (!strcmp(nodeOutputs[startIndex][i], nodes[j])) {
        int nextIndex  = j;
        *retval       += countPaths(nextIndex, fftSeen, dacSeen);
      }
    }
  }
  hsearch(e, ENTER);
  return *retval;
}

int main() {
  FILE *fp = fopen("ex11.input", "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return EXIT_FAILURE;
  }

  // Using hash map for lookups
  hcreate((size_t)(NEL * 2 * 2));
  char buffer[MAX_LINE];
  int nodeIndex  = 0;
  int startIndex = 0;
  while (fgets(buffer, MAX_LINE, fp) != NULL) {
    char *bufPtr = buffer;
    int index    = 0;

    sscanf(bufPtr, "%3s:%n", nodes[nodeIndex], &index);
    if (!strcmp(nodes[nodeIndex], "svr")) {
      startIndex = nodeIndex;
    }

    for (int i = 0; sscanf(bufPtr += index, "%3s%n", nodeOutputs[nodeIndex][i],
                           &index) > 0;
         i++, numOutputs[nodeIndex]++);
    nodeIndex++;
  }

  clock_t start_time = clock();

  int fftSeen         = 0;
  int dacSeen         = 0;
  uint64_t totalPaths = countPaths(startIndex, fftSeen, dacSeen);
  clock_t end_time    = clock();
  double elapsed      = (double)(end_time - start_time) / CLOCKS_PER_SEC;

  printf("Total Paths: %" PRIu64 "\n", totalPaths);
  printf("Elapsed time: %f\n", elapsed);

  // Cleanup
  hdestroy();
  fclose(fp);
}
