#include "priorityQueue.h"
#include "node.h"
#include "helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int solveTSP(int** mat, int n)
{
    int ret = INT_MAX;
    priorityQueue* pq = newPriorityQueue();
    node* node = initialNode(mat, n);
    addpriorityQueue(pq, node);
    while(pq->size > 0 && peekPriorityQUeue(pq)->lowerBound < ret)
    {
        struct node* cur = pollPriorityQueue(pq);
        
        if(cur->avoidFlag)
            reduceScratch(cur);

        //int* pair        
        int* pair = getEdge(cur->matrix, cur->n);

        struct node* avoid = avoidNode(cur, pair, mat);
        addpriorityQueue(pq, avoid);

        commit(cur, pair[0], pair[1]);
        if(cur->n - cur->committedCount == 2)
            ret = fmin(ret, cur->lowerBound);
        else
            addpriorityQueue(pq, cur);
        free(pair);
    }
    return ret;
}

int main(int argc, char* argv[])
{
    FILE* file = fopen(argv[1], "r");
    int n;
    int i;
    fscanf(file, "%d", &n);
    int* flat = (int*)malloc(n * n * sizeof(int));
    for(i = 0; i < n * n; i++)
        fscanf(file, "%d", &(flat[i]));

    int** mat = (int**)malloc(n * sizeof(int*));
    for(i = 0; i < n; i++)
        mat[i] = &(flat[i * n]);
    printf("%d", solveTSP(mat, n));
    free(flat);
    free(mat);
    return 0;
}
