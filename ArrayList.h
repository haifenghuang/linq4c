#ifndef __ARRAYLIST_H__
#define __ARRAYLIST_H__

#ifndef ARRAYLIST_INIT_CAPACITY
#define ARRAYLIST_INIT_CAPACITY 16
#endif

typedef int (*ArrayListCompareFunc)(void *a, void *b);

struct ArrayList {
    void **data;
    int length;    /* number of elements */
    int capacity;  /* capacity of arraylist */
    void (*freeItemFunc)(void *); /* used to free the item when arraylist been destroyed. */
};

typedef struct ArrayList *ArrayList;

ArrayList arrlist_new();
int arrlist_destroy(ArrayList a);

int arrlist_size(ArrayList a);

int arrlist_append(ArrayList a, void *val);
int arrlist_remove(ArrayList a, int index);
int arrlist_insert(ArrayList a, int index, void *val);

void *arrlist_get(ArrayList a, int index);
int arrlist_set(ArrayList a, int index, void *val);

/* In-place reverse */
void arrlist_reverse(ArrayList a);

/* sortOrder(0): Ascending, otherwise: Descending */
void arrlist_sort(ArrayList a, ArrayListCompareFunc compareFn, int sortOrder);

void arrlist_setFreeFunc(ArrayList a, void (*freeItemFunc)(void *));

#endif

