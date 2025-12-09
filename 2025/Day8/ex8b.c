#include "adjListGraph.h"
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_LEN 20
#define NUM_BOXES 1000
#define NUM_DISTS NUM_BOXES *(NUM_BOXES - 1) / 2
#define NUM_CONNECTIONS NUM_DISTS

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

bool allConnected(bool connected[], int length) {
  for (int i = 0; i < length; i++) {
    if (!connected[i]) {
      return false;
    }
  }
  return true;
}

bool allVisited(bool visited[], int length) {
  for (int i = 0; i < length; i++) {
    if (!visited[i]) {
      return false;
    }
  }
  return true;
}

void clearVisited(bool visited[], int length) {
  for (int i = 0; i < length; i++) {
    visited[i] = false;
  }
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
  bool connected[NUM_BOXES] = {0};
  JBox boxList[NUM_BOXES];
  Distance distances[NUM_DISTS];
  int x, y, z;

  for (int i = 0; i < NUM_BOXES; i++) {
    fgets(buffer, MAX_LEN, fp);
    sscanf(buffer, "%d,%d,%d", &x, &y, &z);
    boxList[i] = makeJBox(x, y, z);
  }
  fclose(fp);

  clock_t start_time = clock();
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

  // Keep making connections until graph is complete
  uint64_t final = 0;
  bool found = false;
  while (!found) {
    for (int i = 0; i < NUM_CONNECTIONS; i++) {
      int src = distances[i].srcID;
      int dest = distances[i].destID;
      addEdge(graph, src, dest);
      connected[src] = connected[dest] = true;

      if (allConnected(connected, NUM_BOXES)) {
        // Walk the graph and count circuits
        Node *currNode = graph->array[0];
        int count = DFSCount(graph, 0, visited);
        if (count == NUM_BOXES) {
          final = (uint64_t)boxList[src].x * boxList[dest].x;
          found = true;
          break;
        }

        clearVisited(visited, NUM_BOXES);
      }
    }
  }

  clock_t end_time = clock();
  double elapsed = ((double)end_time - (double)start_time) / CLOCKS_PER_SEC;
  printf("Result of final connection: %" PRIu64 "\n", final);
  printf("Elapsed time: %f\n", elapsed);

  // Cleanup
  freeGraph(graph);
}
