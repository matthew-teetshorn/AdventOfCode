#include "UnionFind/include/UF/UnionFind.h"
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

int main() {
  FILE *fp = fopen("ex8.input", "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return EXIT_FAILURE;
  }

  char buffer[MAX_LEN];
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

  uint64_t final = 0;
  // Implement the UnionFind graph
  UF_Handle uf = UF_Create_Union_Find(NUM_BOXES);
  // Keep making connections until graph is complete
  for (int i = 0; i < NUM_DISTS; i++) {
    int id1 = distances[i].srcID;
    int id2 = distances[i].destID;

    UF_Union(uf, id1, id2);
    if (UF_Is_Complete(uf)) {
      int x1 = boxList[id1].x;
      int y1 = boxList[id1].y;
      int z1 = boxList[id1].z;
      int x2 = boxList[id2].x;
      int y2 = boxList[id2].y;
      int z2 = boxList[id2].z;
      printf("Final coords: (%d, %d, %d) & (%d, %d, %d)\n", x1, y1, z1, x2, y2,
             z2);
      printf("At: id1: %d id2: %d\n", id1, id2);
      final = (uint64_t)boxList[id1].x * (uint64_t)boxList[id2].x;
      break;
    }
  }

  clock_t end_time = clock();
  double elapsed = ((double)end_time - (double)start_time) / CLOCKS_PER_SEC;
  printf("Result of final connection: %" PRIu64 "\n", final);
  printf("Elapsed time: %f\n", elapsed);

  // Cleanup
  UF_Destroy_Union_Find(uf);
}
