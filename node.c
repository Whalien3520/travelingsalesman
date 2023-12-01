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
            arr[0] = getIntArrayList(getListArrayList(parent->includedChains, i), j)[0];
            addIntArrayList(temp, arr, j);
        }
        addListArrayList(ret->includedChains, temp, i);
        ret->matrix[getIntArrayList(temp, temp->size - 1)[0]][getIntArrayList(temp, 0)[0]] = -1;
    }

    ret->excludedPairs = newIntArrayList();
    for(i = 0; i < parent->excludedPairs->size; i++)
    {
        int *temp = (int*)malloc(2 * sizeof(int));
        temp[0] = getIntArrayList(parent->excludedPairs, i)[0];
        temp[1] = getIntArrayList(parent->excludedPairs, i)[1];
        addIntArrayList(ret->excludedPairs, temp, i);
    }
    int* newExclude = (int*)malloc(2 * sizeof(int));
    newExclude[0] = excludeCoords[0];
    newExclude[1] = excludeCoords[1];
    addIntArrayList(ret->excludedPairs, newExclude, ret->excludedPairs->size);

    ret->lowerBound = parent->lowerBound + getMinExclude(parent->matrix, ret->n, excludeCoords[0], excludeCoords[1]);
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
        intArrayList* includedChain = getListArrayList(node->includedChains, i);
        for(j = 0; j < includedChain->size - 1; j++)
        {
            int r = getIntArrayList(includedChain, j)[0];
            int c = getIntArrayList(includedChain, j + 1)[0];
            node->lowerBound += (node->matrix)[r][c];
            for(k = 0; k < node->n; k++)
            {
                node->matrix[r][k] = -1;
                node->matrix[k][c] = -1;
            }
        }
    }
    for(i = 0; i < node->excludedPairs->size; i++)
    {
        int r = getIntArrayList(node->excludedPairs, i)[0];
        int c = getIntArrayList(node->excludedPairs, i)[1];
        node->matrix[r][c] = -1;
    }
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
	if(ret[0] == -1 || ret[1] == -1)
	{
		printf("getEdge is returning a -1");
		exit(1);
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
    int* reduceCandidates = removeCycle(node, x, y);
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
        intArrayList* includedChain = getListArrayList(node->includedChains, i);
        if(getIntArrayList(includedChain, 0)[0] == y)
            headY = i;
        if(getIntArrayList(includedChain, includedChain->size - 1)[0] == x)
            tailX = i;
    }
    if(tailX > -1 && tailX == headY)
    {
        printf("tailX == headY\n");
	int j;
	intArrayList* chain = getListArrayList(node->includedChains, tailX);
	printf("xy:\t%d\t%d\n", x, y);
	printf("mat:%d\n", node->matrix[x][y]);
	printf("list:\t");
	for(j = 0; j < chain->size; j++)
		printf("%d\t", getIntArrayList(chain, j)[0]);
	printf("\n");
        exit(1);
    }
    if(tailX > -1)
    {
        if(headY > -1)
        {
            intArrayList* listX = getListArrayList(node->includedChains, tailX);
            intArrayList* temp = getListArrayList(node->includedChains, headY);
            removeListArrayList(node->includedChains, headY);
            if(temp == NULL || listX == NULL)
            {
                printf("lists were null while joining for removeCycle");
                exit(1);
            }
            while(temp->size > 0)
            {
                int* entry = getIntArrayList(temp, 0);
                removeIntArrayList(temp, 0);
                addIntArrayList(listX, entry, listX->size);
            }
            matX = listX->arr[listX->size - 1][0];
            matY = listX->arr[0][0];
	    freeIntArrayList(temp);
        }
        else
        {
            int* temp = (int*)malloc(sizeof(int));
            temp[0] = y;
            intArrayList* includedChain = getListArrayList(node->includedChains, tailX);
            addIntArrayList(includedChain, temp, includedChain->size);
            matX = y;
            matY = getIntArrayList(getListArrayList(node->includedChains, tailX), 0)[0];
        }
    }
    else
    {
        if(headY > -1)
        {
            int* temp = (int*)malloc(sizeof(int));
            temp[0] = x;
            intArrayList* includedChain = getListArrayList(node->includedChains, headY);
            addIntArrayList(includedChain, temp, 0);
            matX = getIntArrayList(includedChain, includedChain->size - 1)[0];
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
