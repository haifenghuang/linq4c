/*
 * dataStructure.h
 *
 * dataStructure header files
 */

#ifndef _DATASTRUCTURE_H_
#define _DATASTRUCTURE_H_

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

typedef struct Node{
    int mark;
    size_t size;
    void* address;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
    Node* tail;
    size_t linkSize;
} DataStructure;

// DataStructure* _metaData = NULL;

DataStructure* DataStructure_init();
void DataStructure_destroy(DataStructure*);
void Node_insert(DataStructure*, void*, size_t);
void Node_insert_r(DataStructure*, void*, size_t);
void Node_remove(DataStructure*, void*);
void Node_remove_r(DataStructure*, void*);
void* DataStructure_findNode(DataStructure*, void*);
void DataStructure_display(DataStructure*);
void DataStructure_plot(DataStructure*);

#endif
