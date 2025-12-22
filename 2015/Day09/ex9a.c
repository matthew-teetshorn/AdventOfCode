#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Brute force solution with known input size :P
#define MAX_LINE  15
#define NUM_NODES 8

void swap(int *x, int *y) {
  int temp = *x;
  *x       = *y;
  *y       = temp;
}

int permute(int arr[NUM_NODES], int start, int end,
            int dists[NUM_NODES][NUM_NODES]) {
  static int shortest = INT_MAX;
  int currDist        = 0;
  if (start == end) { /* permutation complete */
    for (int i = 0; i < end - 1; i++) {
      currDist += dists[arr[i]][arr[i + 1]];
    }
    if (currDist < shortest) shortest = currDist;
  }

  for (int i = start; i < end; i++) {
    swap(&arr[start], &arr[i]);
    permute(arr, start + 1, end, dists);
    swap(&arr[start], &arr[i]);
  }

  return shortest;
}

int main() {
  FILE *fp = fopen("ex9.input", "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return EXIT_FAILURE;
  }

  char buffer[MAX_LINE];
  int distances[NUM_NODES][NUM_NODES] = {0};
  int node1, node2, dist;
  while (fgets(buffer, MAX_LINE, fp) != NULL) {
    sscanf(buffer, "%d to %d = %d", &node1, &node2, &dist);
    distances[node1][node2] = dist;
    distances[node2][node1] = dist;
  }
  fclose(fp);

  int permutations[NUM_NODES];
  for (int i = 0; i < NUM_NODES; i++) {
    permutations[i] = i;
    // for (int j = 0; j < NUM_NODES; j++) {
    //   printf("[%3d]", distances[i][j]);
    // }
    // printf("\n");
  }

  double start_time = clock();
  int shortest      = permute(permutations, 0, NUM_NODES, distances);
  double end_time   = clock();
  printf("Shortest dist: %d\n", shortest);
  printf("Solved in: %f milliseconds\n",
         (end_time - start_time) / CLOCKS_PER_SEC * 1000);
}
