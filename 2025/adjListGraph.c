#include "adjListGraph.h"
#include <stdlib.h>

Graph *makeGraph(int v) {
  Graph *newGraph = malloc(sizeof(Graph));
  newGraph->numVertices = v;
  newGraph->array = calloc(v, sizeof(Node));

  return newGraph;
}

Node *makeNode(int nextID) {
  Node *newNode = malloc(sizeof(Node));
  newNode->nextID = nextID;
  newNode->next = NULL;
  return newNode;
}

void addEdge(Graph *graph, int srcID, int destID) {
  Node *newNode = makeNode(destID);
  newNode->next = graph->array[srcID];
  graph->array[srcID] = newNode;

  newNode = makeNode(srcID);
  newNode->next = graph->array[destID];
  graph->array[destID] = newNode;
}

void freeGraph(Graph *graph) {
  // Free every node in the adjaceny list array
  Node *current = graph->array[0];
  Node *next;

  while (current != NULL) {
    next = current->next;
    free(current);
    current = next;
  }

  // Free the adjacency list array
  free(graph->array);

  // free the graph
  free(graph);
}
