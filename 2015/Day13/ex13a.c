#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE   70
#define NUM_GUESTS 8

bool allVisited(bool visited[NUM_GUESTS]) {
  for (int i = 0; i < NUM_GUESTS; i++) {
    if (!visited[i]) return false;
  }
  return true;
}

// Brute force walk the graph for max value path
int walk(int start, int current, bool visited[NUM_GUESTS],
         int graph[NUM_GUESTS][NUM_GUESTS], int path[NUM_GUESTS],
         int path_index) {
  int max_weight = INT_MIN;
  int weight     = 0;

  visited[current]   = true;
  path[path_index++] = current;
  if (allVisited(visited)) {
    visited[current] = false;
    return graph[current][start] + graph[start][current];
  }

  for (int i = 0; i < NUM_GUESTS; i++) {
    if (!visited[i]) {
      weight = graph[current][i] + graph[i][current] +
               walk(start, i, visited, graph, path, path_index);
      if (weight > max_weight) {
        max_weight = weight;
      }
    }
  }
  visited[current] = false;

  return max_weight;
}

int main() {
  FILE *fp = fopen("ex13.input", "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return EXIT_FAILURE;
  }

  char buffer[MAX_LINE];
  int graph[NUM_GUESTS][NUM_GUESTS] = {0};
  while (fgets(buffer, MAX_LINE, fp) != NULL) {
    int src, dest, plus_minus, weight;
    sscanf(buffer, "%d would %d %d happiness units by sitting next to %d.",
           &src, &plus_minus, &weight, &dest);
    weight           *= plus_minus;
    graph[src][dest]  = weight;
  }
  fclose(fp);

  for (int i = 0; i < NUM_GUESTS; i++) {
    for (int j = 0; j < NUM_GUESTS; j++) {
      printf("[%3d]", graph[i][j]);
    }
    printf("\n");
  }

  int start                = 0;
  bool visited[NUM_GUESTS] = {false};
  int path[NUM_GUESTS]     = {0};
  int total                = walk(start, start, visited, graph, path, 0);
  printf("Most happiness: %d\n", total);
}
