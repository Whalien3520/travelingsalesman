#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include "node.h"

typedef struct priorityQueue
{
    node** arr;
    int size;
    int cap;
} priorityQueue;

priorityQueue* newPriorityQueue();
void addpriorityQueue(priorityQueue* pq, node* n);
void priorityQueueHelper(priorityQueue* pq, int ind);

node* pollPriorityQueue(priorityQueue* pq);
void minHeapify(priorityQueue* pq, int ind);

node* peekPriorityQUeue(priorityQueue* pq);
void freePriorityQueue(priorityQueue* pq);

#endif