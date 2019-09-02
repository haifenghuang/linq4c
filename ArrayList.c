#include <string.h>
#include "ArrayList.h"
#include <gc.h>

static inline void PTR_SWAP(void **a, void **b) {
    void *t = *a;
    *a = *b;
    *b = t;
}

ArrayList arrlist_new() {
    struct ArrayList *a = GC_malloc(sizeof(struct ArrayList));
    if (a == NULL) {
        return NULL;
    }

    a->data = GC_malloc(sizeof(void *) * ARRAYLIST_INIT_CAPACITY);
    if (a->data == NULL) {
        GC_free(a);
        return NULL;
    }

    a->length = 0;
    a->capacity = ARRAYLIST_INIT_CAPACITY;

    return a;
}

void arrlist_setFreeFunc(ArrayList a, void  (*freeItemFunc)(void *)) {
    a->freeItemFunc = freeItemFunc;
}

static int arrlist_extend(ArrayList a, int newSize) {
    void **tmp = GC_realloc(a->data, sizeof(void *) * newSize);
    if (tmp == NULL) {
        return -1;
    }

    a->data = tmp;

    return 0;
}

static int arrlist_checkSpace(ArrayList a) {
    if (a->length >= a->capacity) {
        a->capacity = a->length + 1;
        a->capacity *= 2;
        return arrlist_extend(a, a->capacity);
    }

    return 0;
}

int arrlist_append(ArrayList a, void *val) {
    if (arrlist_checkSpace(a) < 0) {
        return -1;
    }

    a->data[a->length] = val;
    a->length++;

    return 0;
}

int arrlist_size(ArrayList a) {
    if (a == NULL) {
        return 0;
    }
    return a->length;
}

int arrlist_remove(ArrayList a, int index) {
    if (index < 0 || index >= a->length) {
        return -1;
    }

    for (int i = index; i < (a->length - 1); i++) {
        if (a->freeItemFunc != NULL) {
            a->freeItemFunc(a->data[i]);
        }

        a->data[i] = a->data[i + 1];
    }

    a->length--;

    return 0;
}

int arrlist_insert(ArrayList a, int index, void *val) {
    if (arrlist_checkSpace(a) < 0) {
        return -1;
    }

    for (int i = a->length ; i > index ; i--) {
        a->data[i] = a->data[i - 1];
    }

    a->data[index] = val;
    a->length++;

    return 0;
}

void *arrlist_get(ArrayList a, int index) {
    if (index < 0 || index >= a->length) {
        return NULL;
    }

    return a->data[index];
}

int arrlist_set(ArrayList a, int index, void *val) {
    if (index < 0 || index >= a->length) {
        return -1;
    }

    if (a->freeItemFunc != NULL) {
        a->freeItemFunc(a->data[index]);
    }

    a->data[index] = val;

    return 0;
}

int arrlist_destroy(ArrayList a) {
    if (a == NULL) {
        return -1;
    }
    if (a->freeItemFunc != NULL) {
        for (int i = 0; i < a->length; i++) {
            a->freeItemFunc(a->data[i]);
        }
    }

    GC_free(a->data);
    GC_free(a);

    return 0;
}

void arrlist_reverse(ArrayList a) {
    int lastIndex = a->length - 1;
    for (int i = 0; i < (lastIndex + 1) / 2; i++) {
        PTR_SWAP(&a->data[i], &a->data[lastIndex - i]);
    }
}

static int quicksort_partition(ArrayList list, int left, int right, ArrayListCompareFunc compareFn, int sortOrder) {
    int i, j, pivotIndex;
    void *pivot = NULL;
    j = left;

    pivotIndex = (left + right) / 2;

    pivot = list->data[pivotIndex]; /* use middle as pivot */
    PTR_SWAP(&list->data[pivotIndex], &list->data[right]);

    for (i = left; i < right; i++) {
        if (sortOrder == 0) {
            if ((*compareFn)(list->data[i], pivot) < 0) {
                PTR_SWAP(&list->data[i], &list->data[j++]);
            }
        } else {
            if ((*compareFn)(list->data[i], pivot) >= 0) {
                PTR_SWAP(&list->data[i], &list->data[j++]);
            }
        }
    }
    PTR_SWAP(&list->data[right], &list->data[j]);

    return j;
}

static void arrlist_quicksort(ArrayList list, int leftIndex, int rightIndex, ArrayListCompareFunc compareFn, int sortOrder) {
    if (leftIndex < rightIndex) {
        unsigned int pivotOn = quicksort_partition(list, leftIndex, rightIndex, compareFn, sortOrder);
        arrlist_quicksort(list, leftIndex, pivotOn, compareFn, sortOrder); /* left side */
        arrlist_quicksort(list, pivotOn + 1, rightIndex, compareFn, sortOrder); /* right side */
    }
}

void arrlist_sort(ArrayList a, ArrayListCompareFunc compareFn, int sortOrder) {
    arrlist_quicksort(a, 0, a->length - 1, compareFn, sortOrder);
}

