#include "adjListGraph.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_LEN 20
#define NUM_BOXES 1000
#define NUM_CONNECTIONS 1000
#define NUM_DISTS NUM_BOXES *(NUM_BOXES - 1) / 2

typedef struct {
  double distance;
  int srcID;
  int destID;
} Distance;

typedef struct {
  int x, y, z;
} JBox;

JBox makeJBox(int x, int y, int z) {
  JBox newBox;
  newBox.x = x;
  newBox.y = y;
  newBox.z = z;
  return newBox;
}

double getDistance(JBox *boxes, int srcID, int destID) {
  double distance = sqrt(pow((boxes[srcID].x - boxes[destID].x), 2) +
                         pow(boxes[srcID].y - boxes[destID].y, 2) +
                         pow(boxes[srcID].z - boxes[destID].z, 2));
  return distance;
}

int compareDists(const void *a, const void *b) {
  Distance dA = *(Distance *)a;
  Distance dB = *(Distance *)b;

  if (dA.distance < dB.distance) {
    return -1;
  } else if (dA.distance == dB.distance) {
    return 0;
  } else {
    return 1;
  }
}

void printAdjList(Graph *graph) {
  for (int i = 0; i < NUM_BOXES; i++) {
    printf("%d: ", i);
    Node *currNode = graph->array[i];
    if (currNode == NULL) {
      printf("NULL");
    } else {
      while (currNode != NULL) {
        printf("[%d]", currNode->nextID);
        currNode = currNode->next;
      }
    }
    printf("\n");
  }
}

// Visit all nodes reachable from currIndex and return the number
int DFSCount(Graph *graph, int currIndex, bool visited[]) {
  Node *currNode = graph->array[currIndex];
  visited[currIndex] = true;
  int count = 1;

  while (currNode) {
    int nextNode = currNode->nextID;
    if (!visited[nextNode]) {
      count += DFSCount(graph, nextNode, visited);
    }
    currNode = currNode->next;
  }

  return count;
}

int main() {
  FILE *fp = fopen("ex8.input", "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return EXIT_FAILURE;
  }

  char buffer[MAX_LEN];
  Graph *graph = makeGraph(NUM_BOXES);
  bool visited[NUM_BOXES] = {0};
  JBox boxList[NUM_BOXES];
  Distance distances[NUM_DISTS];
  int x, y, z;

  for (int i = 0; i < NUM_BOXES; i++) {
    fgets(buffer, MAX_LEN, fp);
    sscanf(buffer, "%d,%d,%d", &x, &y, &z);
    boxList[i] = makeJBox(x, y, z);
  }
  fclose(fp);

  // Calculate and store all distances between points
  int index = 0;
  for (int i = 0; i < NUM_BOXES - 1; i++) {
    for (int j = i + 1; j < NUM_BOXES; j++) {
      distances[index] = (Distance){
          .distance = getDistance(boxList, i, j), .srcID = i, .destID = j};
      index++;
    }
  }

  // Sort the distances
  qsort(distances, NUM_DISTS, sizeof(Distance), compareDists);

  // Make the N shortest connections
  for (int i = 0; i < NUM_CONNECTIONS; i++) {
    int src = distances[i].srcID;
    int dest = distances[i].destID;
    addEdge(graph, src, dest);
  }

  // Walk the graph and count circuits
  int first = 0, second = 0, third = 0;
  for (int i = 0; i < NUM_BOXES; i++) {
    Node *currNode = graph->array[i];
    if (currNode == NULL || visited[i]) {
      continue;
    }
    int count = DFSCount(graph, i, visited);
    if (count > first) {
      third = second;
      second = first;
      first = count;
    } else if (count > second) {
      third = second;
      second = count;
    } else if (count > third) {
      third = count;
    }
  }

  // printAdjList(graph);
  printf("first: %d, second: %d, third: %d\n", first, second, third);
  printf("total: %d\n", first * second * third);

  // Cleanup
  freeGraph(graph);
}
