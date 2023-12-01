#include "listArrayList.h"
#include "intArrayList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void addListArrayList(listArrayList* list, intArrayList* entry, int index)
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
        list->arr = (intArrayList**)malloc(sizeof(intArrayList*));
        if(list->arr == NULL)
        {
            printf("malloc initial listArrayList array failed");
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
            list->arr = (intArrayList**)realloc(list->arr, 2 * list->cap * sizeof(intArrayList*));
            if(list->arr == NULL)
            {
                printf("realloc intArrayList failed");
                exit(1);
            }
            list->cap = 2 * list->cap;
        }
        memmove(&(list->arr[index + 1]), &(list->arr[index]), (list->size - index) * sizeof(intArrayList*));
        list->arr[index] = entry;
        list->size += 1;
    }
}
intArrayList* removeListArrayList(listArrayList* list, int index)
{
    if(list == NULL)
    {
        printf("Attempted to remove from null listArrayList\n");
        exit(1);
    }
    if(list->size == 0)
    {
        printf("Attempted to remove from empty listArrayList\n");
        exit(1);
    }
    if(index < 0 || index >= list->size)
    {
        printf("Attempted to remove from outside listArrayList bounds\n");
        exit(1);
    }
    intArrayList* ret = list->arr[index];
    memmove(&(list->arr[index]), &(list->arr[index + 1]), (list->size - index - 1) * sizeof(intArrayList*));
    list->size--;
    return ret;
}
void freeListArrayList(listArrayList* list)
{
    if(list == NULL)
    {
        printf("attempted to free null listArrayList");
        exit(1);
    }
    int i;
    for(i = 0; i < list->size; i++)
        freeIntArrayList(list->arr[i]);
    free(list->arr);
    free(list);
}
listArrayList* newListArrayList()
{
    listArrayList *ret = (listArrayList*)malloc(sizeof(listArrayList));
    if(ret == NULL)
    {
        printf("malloc listArrayList failed");
        exit(1);
    }
    ret->cap = 0;
    ret->size = 0;
    ret->arr = NULL;
    return ret;
}
