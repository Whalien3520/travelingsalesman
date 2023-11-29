#ifndef LISTARRAYLIST_H
#define LISTARRAYLIST_H

#include "intArrayList.h"

typedef struct listArrayList
{
    intArrayList **arr;
    int size, cap;
} listArrayList;

void addListArrayList(listArrayList* list, intArrayList* entry, int index);
intArrayList* removeListArrayList(listArrayList* list, int index);
void freeListArrayList(listArrayList* list);
listArrayList* newListArrayList();

#endif
