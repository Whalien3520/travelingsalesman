#include "priorityQueue.h"
#include <stdio.h>
#include <stdlib.h>

// adapted from https://www.geeksforgeeks.org/c-program-to-implement-min-heap/

priorityQueue* newPriorityQueue()
{
    priorityQueue* ret = (priorityQueue*)malloc(sizeof(priorityQueue));
    if(ret == NULL)
    {
        printf("malloc priorityQueue failed");
        exit(1);
    }
    ret->size = 0;
    ret->cap = 0;
    return ret;
}
void addpriorityQueue(priorityQueue* pq, node* n)
{
    if(pq == NULL)
    {
        printf("Attempted to add to null priorityQueue\n");
        exit(1);
    }
    if(n == NULL)
    {
        printf("Attempted to add null to priorityQueue\n");
        exit(1);
    }

    if(pq->cap == 0)
    {
        pq->arr = (node**)malloc(sizeof(node*));
        if(pq->arr == NULL)
        {
            printf("malloc initial priorityQueue array failed");
            exit(1);
        }
        pq->cap = 1;
        pq->arr[0] = n;
        pq->size = 1;
    }
    else
    {
        if(pq->size == pq->cap)
        {
            pq->arr = (node**)realloc(pq->arr, 2 * pq->cap * sizeof(node*));
            if(pq->arr == NULL)
            {
                printf("realloc priorityQueue failed");
                exit(1);
            }
            pq->cap = 2 * pq->cap;
        }
        pq->arr[pq->size] = n;
        priorityQueueHelper(pq, pq->size);
        pq->size++;
    }
}
void priorityQueueHelper(priorityQueue* pq, int ind)
{
    int parent = (ind - 1 ) / 2;
    if(pq->arr[parent]->lowerBound > pq->arr[ind]->lowerBound)
    {
        node* temp = pq->arr[parent];
        pq->arr[parent] = pq->arr[ind];
        pq->arr[ind] = temp;

        priorityQueueHelper(pq, parent);
    }
}
node* pollPriorityQueue(priorityQueue* pq)
{
    if(pq == NULL)
    {
        printf("attempted to poll from null priorityQueue");
        exit(1);
    }
    if(pq->size == 0)
    {
        printf("attempted to poll from empty priorityQueue");
        exit(1);
    }
    node* ret = pq->arr[0];
    pq->arr[0] = pq->arr[pq->size - 1];
    pq->size--;
    minHeapify(pq, 0);
    return ret;
}
void minHeapify(priorityQueue* pq, int ind)
{
    int l = ind * 2 + 1, r = ind * 2 + 2, min = ind;
    if(l >= pq->size || l < 0)
        l = -1;
    if(r >= pq->size || r < 0)
        r = -1;

    if(l != -1 && pq->arr[l]->lowerBound < pq->arr[ind]->lowerBound)
        min = l;
    if(r != -1 && pq->arr[r]->lowerBound < pq->arr[ind]->lowerBound)
        min = r;
    
    if(min != ind)
    {
        node* temp = pq->arr[min];
        pq->arr[min] = pq->arr[ind];
        pq->arr[ind] = temp;

        minHeapify(pq, min);
    }
}

node* peekPriorityQUeue(priorityQueue* pq)
{
    return pq->arr[0];
}
void freePriorityQueue(priorityQueue* pq)
{
    //printf("pq size: %d\n", pq->size);
    int i;
    for(i = 0; i < pq->size; i++)
    {
	//printf("%d\n", i);
        freeNode(pq->arr[i]);
    }
    free(pq->arr);
    free(pq);
}
