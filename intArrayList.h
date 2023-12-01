#ifndef INTARRAYLIST_H
#define INTARRAYLIST_H

typedef struct intArrayList
{
    int **arr;
    int size, cap;
} intArrayList;

void addIntArrayList(intArrayList* list, int* entry, int index);
void removeIntArrayList(intArrayList* list, int index);
int* getIntArrayList(intArrayList* list, int ind);
void freeIntArrayList(intArrayList* list);
intArrayList* newIntArrayList();

#endif
