#ifndef __HASH_MAP_H__
#define __HASH_MAP_H__

#include <stdlib.h>

#ifndef HASHMAP_INIT_CAPACITY
#define HASHMAP_INIT_CAPACITY 16
#endif

typedef struct {
    void *key;
    void *value;
} KeyValuePair;

struct HashMap {
    KeyValuePair **data;
    int length;
    int capacity;
};

typedef struct HashMap *HashMap;

HashMap hashmap_new(void);
void hashmap_destroy(HashMap hm);

void hashmap_add(HashMap hm, KeyValuePair *value);
void hashmap_remove(HashMap hm, int index);

KeyValuePair *hashmap_get(HashMap hm, int index);

int hashmap_size(HashMap hm);

#endif

