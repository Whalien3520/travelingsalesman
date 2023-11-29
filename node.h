#ifndef NODE_H
#define NODE_H

#include "listArrayList.h"
#include "intArrayList.h"

typedef struct node {
    int **matrix;
    int n;
    listArrayList* includedChains;
    intArrayList* excludedPairs;
    int lowerBound;
    int committedCount;
    int avoidFlag;
} node;

node* initialNode(int** matrix, int n);
node* avoidNode(node* parent, int excludeCoords[2], int** matrix);
int** dupeMatrix(int** matrix, int n);
void freeNode(node* node);

void reduceScratch(node* node);
void reduceTake(node* node, intArrayList* rows, intArrayList* cols);

int* getEdge(int** matrix, int n);
int getMinExclude(int** matrix, int n, int x, int y);

void commit(node* node, int x, int y);
int* removeCycle(node* node, int x, int y);

#endif