#pragma once

#include <stdlib.h>

typedef struct Node {
  int nextID;
  struct Node *next;
} Node;

typedef struct {
  int numVertices;
  struct Node **array;
} Graph;

Graph *makeGraph(int v);

Node *makeNode(int nextID);

void addEdge(Graph *graph, int srcID, int destID);

void freeGraph(Graph *graph);
