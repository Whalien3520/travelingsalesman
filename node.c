#include "node.h"
#include "helper.h"
#include "listArrayList.h"
#include "intArrayList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

node* initialNode(int** matrix, int n)
{
    node* ret = (node*)malloc(sizeof(node));
    ret->matrix = dupeMatrix(matrix, n);
    ret->n = n;
    ret->includedChains = newListArrayList();
    ret->excludedPairs = newIntArrayList();
    ret->lowerBound = 0;
    ret->committedCount = 0;
    reduceScratch(ret);
    return ret;
}
node* avoidNode(node* parent, int excludeCoords[2], int** matrix)
{
    node* ret = (node*)malloc(sizeof(node));
    ret->matrix = dupeMatrix(matrix, parent->n);
    ret->n = parent->n;

    ret->includedChains = newListArrayList();
    int i;
    for(i = 0; i < parent->includedChains->size; i++)
    {
        intArrayList* temp = newIntArrayList();
        int j;
        for(j = 0; j < parent->includedChains->arr[i]->size; j++)
        {
            int* arr = (int*)malloc(sizeof(int));
            arr[0] = parent->includedChains->arr[i]->arr[j][0];
            addIntArrayList(temp, arr, j);
        }
        addListArrayList(ret->includedChains, temp, i);
    }

    ret->excludedPairs = newIntArrayList();
    for(i = 0; i < parent->excludedPairs->size; i++)
    {
        int *temp = (int*)malloc(2 * sizeof(int));
        temp[0] = (parent->excludedPairs->arr[i])[0];
        temp[1] = (parent->excludedPairs->arr[i])[1];
        addIntArrayList(ret->excludedPairs, temp, i);
    }
    int* newExclude = (int*)malloc(2 * sizeof(int));
    newExclude[0] = excludeCoords[0];
    newExclude[1] = excludeCoords[1];
    addIntArrayList(ret->excludedPairs, newExclude, parent->excludedPairs->size);

    ret->lowerBound = parent->lowerBound + getMinExclude(ret->matrix, ret->n, excludeCoords[0], excludeCoords[1]);
    ret->committedCount = parent->committedCount;
    ret->avoidFlag = 1;

    return ret;
}
int** dupeMatrix(int** matrix, int n)
{
    int** ret = (int**)malloc(n * sizeof(int*));
    int i;
    for(i = 0; i < n; i++)
    {
        ret[i] = (int*)malloc(n * sizeof(int));
        memcpy(ret[i], matrix[i], n * sizeof(int));
    }
    return ret;
}
void freeNode(node* node)
{
    int i;
    for(i = 0; i < node->n; i++)
        free(node->matrix[i]);
    free(node->matrix);
    freeListArrayList(node->includedChains);
    freeIntArrayList(node->excludedPairs);
    free(node);
}

void reduceScratch(node* node)
{
    node->avoidFlag = 0;
    node->lowerBound = 0;
    int i, j, k;

    for(i = 0; i < node->includedChains->size; i++)
    {
        intArrayList* includedChain = node->includedChains->arr[i];
        for(j = 0; j < includedChain->size - 1; j++)
        {
            node->lowerBound += (node->matrix)[includedChain->arr[j][0]][includedChain->arr[j + 1][0]];
            for(k = 0; k < node->n; k++)
            {
                node->matrix[includedChain->arr[j][0]][k] = -1;
                node->matrix[k][includedChain->arr[j + 1][0]] = -1;
            }
        }
    }
    for(i = 0; i < node->excludedPairs->size; i++)
    {
        node->matrix[node->excludedPairs->arr[i][0]][node->excludedPairs->arr[i][1]] = -1;
    }
    //printf("Checkpoint!\n");
    for(i = 0; i < node->n; i++)
    {
        int min = INT_MAX;
        for(j = 0; j < node->n; j++)
        {
            if(node->matrix[i][j] == -1)
                continue;
            min = minInts(min, node->matrix[i][j]);
            if(min == 0)
                break;
        }
        if(min == 0 || min == INT_MAX)
            continue;
        node->lowerBound += min;
        for(j = 0; j < node-> n; j++)
        {
            if(node->matrix[i][j] == -1)
                continue;
            node->matrix[i][j] -= min;
        }
    }
    for(i = 0; i < node->n; i++)
    {
        int min = INT_MAX;
        for(j = 0; j < node->n; j++)
        {
            if(node->matrix[j][i] == -1)
                continue;
            min = minInts(min, node->matrix[j][i]);
            if(min == 0)
                break;
        }
        if(min == 0 || min == INT_MAX)
            continue;
        node->lowerBound += min;
        for(j = 0; j < node-> n; j++)
        {
            if(node->matrix[j][i] == -1)
                continue;
            node->matrix[j][i] -= min;
        }
    }
}
void reduceTake(node* node, intArrayList* rows, intArrayList* cols)
{
    int r, c;
    for(r = 0; r < rows->size; r++)
    {
        int min = INT_MAX;
        for(c = 0; c < node->n; c++)
        {
            if(node->matrix[rows->arr[r][0]][c] == -1)
                continue;
            min = minInts(min, node->matrix[rows->arr[r][0]][c]);
            if(min == 0)
                break;
        }
        if(min == 0 || min == INT_MAX)
            continue;
        node->lowerBound += min;
        for(c = 0; c < node->n; c++)
        {
            if(node->matrix[rows->arr[r][0]][c] == -1)
                continue;
            node->matrix[rows->arr[r][0]][c] -= min;
        }
    }
    for(c = 0; c < cols->size; c++)
    {
        int min = INT_MAX;
        for(r = 0; r < node->n; r++)
        {
            if(node->matrix[r][cols->arr[c][0]] == -1)
                continue;
            min = minInts(min, node->matrix[r][cols->arr[c][0]]);
            if(min == 0)
                break;
        }
        if(min == 0 || min == INT_MAX)
            continue;
        node->lowerBound += min;
        for(r = 0; r < node->n; r++)
        {
            if(node->matrix[r][cols->arr[c][0]] == -1)
                continue;
            node->matrix[r][cols->arr[c][0]] -= min;
        }
    }
}
// RETURN NEEDS TO BE FREED
int* getEdge(int** matrix, int n)
{
    int* ret = (int*)malloc(2 * sizeof(int));
    int max = -1;
    int i, j;
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < n; j++)
        {
            if(matrix[i][j] != 0)
                continue;
            int tempMax = getMinExclude(matrix, n, i, j);
            if(tempMax > max)
            {
                max = tempMax;
                ret[0] = i;
                ret[1] = j;
            }
        }
    }
    return ret;
}
int getMinExclude(int** matrix, int n, int x, int y)
{
    int rmax = INT_MAX, cmax = INT_MAX;
    int i;
    for(i = 0; i < n; i++)
    {
        if(matrix[x][i] > -1 && i != y)
            rmax = minInts(rmax, matrix[x][i]);
        if(matrix[i][y] > -1 && i != x)
            cmax = minInts(cmax, matrix[i][y]);
    }
    return rmax + cmax;
}

void commit(node* node, int x, int y)
{
    node->committedCount++;
    intArrayList *reduceRows = newIntArrayList(), *reduceCols = newIntArrayList();
    int i;
    for(i = 0; i < node->n; i++)
    {
        if(node->matrix[x][i] == 0)
        {
            int* arr = (int*)malloc(sizeof(int));
            arr[0] = i;
            addIntArrayList(reduceRows, arr, reduceRows->size);
        }
        node->matrix[x][i] = -1;
        if(node->matrix[i][y] == 0)
        {
            int* arr = (int*)malloc(sizeof(int));
            arr[0] = i;
            addIntArrayList(reduceCols, arr, reduceCols->size);
        }
        node->matrix[i][y] = -1;
    }
    //printf("Checkpoint1\n");
    int* reduceCandidates = removeCycle(node, x, y);
    //printf("Checkpoint2\n");
    if(reduceCandidates[0] > -1)
    {
        int* arrx = (int*)malloc(sizeof(int));
        arrx[0] = reduceCandidates[0];
        addIntArrayList(reduceRows, arrx, reduceRows->size);
        int* arry = (int*)malloc(sizeof(int));
        arry[0] = reduceCandidates[1];
        addIntArrayList(reduceCols, arry, reduceCols->size);
    }
    free(reduceCandidates);
    reduceTake(node, reduceRows, reduceCols);
    freeIntArrayList(reduceRows);
    freeIntArrayList(reduceCols);
}
// RETURN NEEDS TO BE FREED
int* removeCycle(node* node, int x, int y)
{
    int tailX = -1, headY = -1;
    int matX = -1, matY = -1;
    int* ret = (int*)malloc(2 * sizeof(int));
    ret[0] = -1;
    ret[1] = -1;
    int i;
    for(i = 0; i < node->includedChains->size; i++)
    {
        if(node->includedChains->arr[i]->arr[0][0] == y)
            headY = i;
        if(node->includedChains->arr[i]->arr[node->includedChains->arr[i]->size - 1][0] == x)
            tailX = i;
    }
    if(tailX > -1)
    {
        if(headY > -1)
        {
		//printf("Flag1\n");
            intArrayList* listX = node->includedChains->arr[tailX];
            intArrayList* temp = removeListArrayList(node->includedChains, headY);
            if(temp == NULL || listX == NULL)
            {
                printf("lists were null while joining for removeCycle");
                exit(1);
            }
            int j;
		//printf("Flag2\n");
            //for(j = temp->size - 1; j >= 0; j--)
            while(temp->size > 0)
            {
                for(j = 0; j < temp->size; j++)
                    printf("%d\t", temp->arr[j][0]);
                printf("\n");
                addIntArrayList(listX, removeIntArrayList(temp, 0), listX->size);
            }
            matX = listX->arr[listX->size - 1][0];
            matY = listX->arr[0][0];
	    freeIntArrayList(temp);
        }
        else
        {
            int* temp = (int*)malloc(sizeof(int));
            temp[0] = y;
            addIntArrayList(node->includedChains->arr[tailX], temp, node->includedChains->arr[tailX]->size);
            matX = y;
            matY = node->includedChains->arr[tailX]->arr[0][0];
        }
    }
    else
    {
        if(headY > -1)
        {
            int* temp = (int*)malloc(sizeof(int));
            temp[0] = x;
            addIntArrayList(node->includedChains->arr[headY], temp, 0);
            matX = node->includedChains->arr[headY]->arr[node->includedChains->arr[headY]->size - 1][0];
            matY = x;
        }
        else
        {
            int *tempX = (int*)malloc(sizeof(int)), *tempY = (int*)malloc(sizeof(int));
            tempX[0] = x;
            tempY[0] = y;
            intArrayList* tempList = newIntArrayList();
            addIntArrayList(tempList, tempX, 0);
            addIntArrayList(tempList, tempY, 1);
            addListArrayList(node->includedChains, tempList, node->includedChains->size);
            matX = y;
            matY = x;
        }
    }
    if(matX == -1 || matY == -1)
    {
        printf("matX or matY was never changed from -1");
        exit(1);
    }
    if(node->matrix[matX][matY] == 0)
    {
        ret[0] = matX;
        ret[1] = matY;
    }
    node->matrix[matX][matY] = -1;
    return ret;
}
