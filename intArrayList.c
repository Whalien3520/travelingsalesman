#include "intArrayList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void addIntArrayList(intArrayList* list, int* entry, int index)
{
    if(list == NULL)
    {
        printf("Attempted to add to null listArrayList\n");
        exit(1);
    }
    if(entry == NULL)
    {
        printf("Attempted to add null to listArrayList\n");
        exit(1);
    }
    if(index < 0 || index > list->size + 1)
    {
        printf("Attempted to add element beyond listArrayList bounds\n");
        exit(1);
    }

    if(list->size == 0)
    {
        list->arr = (int**)malloc(sizeof(int*));
        if(list->arr == NULL)
        {
            printf("malloc initial intArrayList array failed");
            exit(1);
        }
        list->size = 1;
        list->cap = 1;
        list->arr[0] = entry;
    }
    else
    {
        if(list->size == list->cap)
        {
            list->arr = (int**)realloc(list->arr, 2 * list->cap * sizeof(int*));
            if(list->arr == NULL)
            {
                printf("realloc intArrayList failed");
                exit(1);
            }
            list->cap = 2 * list->cap;
        }
        if(index < list->size)
            memmove(&(list->arr[index + 1]), &(list->arr[index]), (list->size - index) * sizeof(int*));
        list->arr[index] = entry;
        list->size += 1;
    }
}
int* removeIntArrayList(intArrayList* list, int index)
{
    if(list == NULL)
    {
        printf("Attempted to remove from null intArrayList\n");
        exit(1);
    }
    if(list->size == 0)
    {
        printf("Attempted to remove from empty intArrayList\n");
        exit(1);
    }
    if(index < 0 || index >= list->size)
    {
        printf("Attempted to remove from outside intArrayList bounds\n");
        exit(1);
    }
    int* ret = list->arr[index];
    memmove(&(list->arr[index]), &(list->arr[index + 1]), (list->size - index - 1) * sizeof(int*));
    list->size -= 1;
    return ret;
}
intArrayList* newIntArrayList()
{
    intArrayList *ret = (intArrayList*)malloc(sizeof(intArrayList));
    if(ret == NULL)
    {
        printf("malloc intarraylist failed");
        exit(1);
    }
    ret->cap = 0;
    ret->size = 0;
    ret->arr = NULL;
    return ret;
}
void freeIntArrayList(intArrayList* list)
{
    if(list == NULL)
    {
        printf("attempted to free null intArrayList");
        exit(1);
    }
    int i;
    for(i = 0; i < list->size; i++)
    {
        //printf("\t%d\n", i);
        free(list->arr[i]);
    }
    if(list->arr != NULL)
        free(list->arr);
    free(list);
}
