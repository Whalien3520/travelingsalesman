#ifndef INTARRAYLIST_H
#define INTARRAYLIST_H

typedef struct intArrayList
{
    int **arr;
    int size, cap;
} intArrayList;

void addIntArrayList(intArrayList* list, int* entry, int index);
int* removeIntArrayList(intArrayList* list, int index);
void freeIntArrayList(intArrayList* list);
intArrayList* newIntArrayList();

#endif