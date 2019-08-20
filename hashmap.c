#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "hashmap.h"
#include "malloc.h"

HashMap hashmap_new() {
    HashMap hm = gc_malloc(sizeof(struct HashMap));
    hm->length = 0;
    hm->capacity = HASHMAP_INIT_CAPACITY;
    hm->data = gc_malloc(sizeof(KeyValuePair *) * hm->capacity);
    return hm;
}

void hashmap_destroy(HashMap hm) {
    for (int index = 0; index < hm->length; index++) {
        gc_free(hm->data[index]->key);
        gc_free(hm->data[index]->value);
        gc_free(hm->data[index]);
    }
}

void hashmap_add(HashMap hm, KeyValuePair *value) {
    if (hm->length == hm->capacity) {
        hm->capacity += HASHMAP_INIT_CAPACITY;
        hm->data = gc_realloc(hm->data, sizeof(KeyValuePair *) * hm->capacity);
    }

    hm->data[hm->length] = value;
    hm->length++;
}

void hashmap_remove(HashMap hm, int index) {
    if (index <= hm->length) {
        gc_free(hm->data[index]->key);
        gc_free(hm->data[index]->value);
        gc_free(hm->data[index]);
    }
}

KeyValuePair *hashmap_get(HashMap hm, int index) {
    if (index <= hm->length) {
        return hm->data[index];
    }
    return NULL;
}

int hashmap_size(HashMap hm) {
    return hm->length;
}

