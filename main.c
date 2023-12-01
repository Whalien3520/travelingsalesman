#include "priorityQueue.h"
#include "node.h"
#include "helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int solveTSP(int** mat, int n)
{
	//printf("Hello World!\n");
    int ret = INT_MAX;
    priorityQueue* pq = newPriorityQueue();
    node* node = initialNode(mat, n);
    addpriorityQueue(pq, node);
    //printf("Hello World2!\n");
    while(pq->size > 0 && peekPriorityQUeue(pq)->lowerBound < ret)
    {
        struct node* cur = pollPriorityQueue(pq);

        if(cur->avoidFlag)
            reduceScratch(cur);

        int* pair = getEdge(cur->matrix, cur->n);
        struct node* avoid = avoidNode(cur, pair, mat);

        addpriorityQueue(pq, avoid);

        commit(cur, pair[0], pair[1]);
        if(cur->n - cur->committedCount == 2)
        {
            ret = minInts(ret, cur->lowerBound);
            freeNode(cur);
        }
        else
            addpriorityQueue(pq, cur);
        free(pair);
    }
    freePriorityQueue(pq);
    return ret;
}

int main(int argc, char* argv[])
{
    FILE* file = fopen("data.txt", "r");
    int n;
    int i;
    fscanf(file, "%d", &n);
    int* flat = (int*)malloc(n * n * sizeof(int));
    for(i = 0; i < n * n; i++)
        fscanf(file, "%d", &(flat[i]));
    int** mat = (int**)malloc(n * sizeof(int*));
    for(i = 0; i < n; i++)
        mat[i] = &(flat[i * n]);
    printf("%d\n", solveTSP(mat, n));
    free(mat);
    free(flat);
    fclose(file);
    return 0;
}
