#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

#include <stddef.h>
#include <pthread.h>
#include "mark_and_sweep.h"

typedef struct llNode {
    long threadID;
    long stack_top;
    long stack_bottom;
    struct llNode* next;
} llNode;


void ll_insertNode(llNode** head, size_t threadID, size_t stack_top, size_t stack_bottom);
void ll_destroy(llNode** head);
void ll_removeNode(llNode** head, size_t threadID);
llNode* ll_findNode(llNode* head, size_t threadID);
void ll_updateStackTop(llNode*, size_t, size_t);

void ll_iterate(llNode* head, DataStructure* _metaData);



#endif
