#include "linkedList.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>


llNode* ll_findNode(llNode* head, size_t threadID){

    llNode* node = head;

    while(node){
        if(node->threadID == threadID){
            return node;
        }
        node = node->next; 
    }

    return NULL;

}

void ll_updateStackTop(llNode* head, size_t threadID, size_t newStackTop){

    llNode* node = head;

    while(node){
        if(node->threadID == threadID){
            node->stack_top = newStackTop;
            return;
        }
        node = node->next;
    }


}



void ll_insertNode(llNode** head, size_t threadID, size_t stack_top, size_t stack_bottom){

    llNode* n = (llNode*)malloc(sizeof(llNode));

    n->threadID = threadID;
    n->stack_top = stack_top;
    n->stack_bottom = stack_bottom;

    // list is empty
    if(*head == NULL){
        *head = n;
        n->next = NULL;
        return;
    } else {
        n->next = *head;
        *head = n;
        return;
    }
}


void ll_destroy(llNode** head){
    llNode* current = *head;
    llNode* temp = NULL;
    while(current != NULL){
        temp = current->next;
        current->next = NULL;
        free(current);
        current = temp;
    }

}

void ll_removeNode(llNode** head, size_t threadID){
    llNode *temp = *head, *prev = NULL;

    while(temp){
        if(temp->threadID == threadID){
            if(prev)
                prev->next = temp->next;
            if(temp == *head)
                *head = temp->next;
            temp->next = NULL;
            free(temp);
            break;
        }   
        if(prev == NULL)
            prev = *head;
        else
            prev = prev->next;
        temp = temp->next;
    }
}

void ll_iterate(llNode* head, DataStructure* _metaData){

    llNode* node = head;
    while(node){
        mark_on_stack(_metaData, node->stack_top, node->stack_bottom, node->threadID);
        node = node->next;
    }
}    
