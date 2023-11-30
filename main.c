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
	//printf("Hello World3!\n");
        struct node* cur = pollPriorityQueue(pq);
	//printf("Hello World4!\n");        
        if(cur->avoidFlag)
            reduceScratch(cur);
	//printf("Hello World5!\n");
        //int* pair        
        int* pair = getEdge(cur->matrix, cur->n);
	//printf("Hello World6!\n");
        struct node* avoid = avoidNode(cur, pair, mat);
        addpriorityQueue(pq, avoid);
	//printf("Hello World7!\n");
        commit(cur, pair[0], pair[1]);
	//printf("Hello World8!\n");
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
    //printf("Hello World!\n");
    //printf("%s\n", argv[0]);
    //printf("%s\n", argv[1]);
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
    printf("%d", solveTSP(mat, n));
    free(mat);
    free(flat);
    return 0;
}
