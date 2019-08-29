//good lord I don't know if this is correct
/*c.c
 *
 * malloc.h implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include "malloc.h"
#include "mark_and_sweep.h"
#include "linkedList.h"

DataStructure* _metaData = NULL;
static llNode* pthread_ll_head = NULL;

static pthread_mutex_t _SIGNAL_MUTEX = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t _MALLOC_MUTEX = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t _SIGNAL_CV = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t mtx_1 = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t ll_mtx = PTHREAD_MUTEX_INITIALIZER;

void SIGNALHANDLER();

#if 0
#undef malloc
#undef calloc
#undef realloc
#undef free
#undef pthread_join
#endif

static long _numThreads;
static pthread_cond_t _mainThreadCond = PTHREAD_COND_INITIALIZER;

int gc_pthread_join(pthread_t thread, void** retval){
    pthread_mutex_lock(&mtx_1);
    long ret = pthread_join(thread, retval);
    pthread_mutex_lock(&ll_mtx);
    ll_removeNode(&pthread_ll_head, (long)thread);
    pthread_mutex_unlock(&ll_mtx);
    pthread_mutex_unlock(&mtx_1);
    return ret;
}

void updateStackTop(){

    pthread_mutex_lock(&ll_mtx);
    llNode* node = ll_findNode(pthread_ll_head, pthread_self());
    pthread_mutex_unlock(&ll_mtx);

    int i = 0;

    node->stack_top = (size_t)(&i);

}


void SIGNALHANDLER()
{
    pthread_mutex_lock(&_SIGNAL_MUTEX);

    pthread_mutex_lock(&ll_mtx);
    int i = 0;
    ll_updateStackTop(pthread_ll_head, pthread_self(), (size_t)(&i));
    pthread_mutex_unlock(&ll_mtx);

    _numThreads--;

    while(_CLEAN_FLAG)
    {
        pthread_cond_wait(&_SIGNAL_CV, &_SIGNAL_MUTEX);
    }
    pthread_mutex_unlock(&_SIGNAL_MUTEX);
}

void* clean_helper()
{
    pthread_mutex_lock(&_SIGNAL_MUTEX);
    _CLEAN_FLAG = 1;
    _numThreads = 0;
    pthread_mutex_unlock(&_SIGNAL_MUTEX);
    pthread_mutex_lock(&ll_mtx);
    llNode* current = pthread_ll_head;
    if(current != NULL)
    {
        pthread_t calling_thread_pid = pthread_self();
        while(current != NULL)
        {
            pthread_t pid = (pthread_t)current->threadID;
            if(pid != calling_thread_pid) //put all other threads to sleep
            {
                pthread_mutex_lock(&_SIGNAL_MUTEX);
                _numThreads++;
                pthread_mutex_unlock(&_SIGNAL_MUTEX);
                pthread_kill(pid, SIGUSR1); 
            }
            current = current->next;
        }
    }
    pthread_mutex_unlock(&ll_mtx);

    pthread_mutex_lock(&_SIGNAL_MUTEX);
    while(_numThreads)
        pthread_cond_wait(&_mainThreadCond, &_SIGNAL_MUTEX);
    pthread_mutex_unlock(&_SIGNAL_MUTEX);

    llNode* node = pthread_ll_head;
    mark_all_on_stack(_metaData);
    pthread_mutex_lock(&mtx_1);
    while(node){
        mark_on_stack(_metaData, node->stack_top, node->stack_bottom, node->threadID);
        node = node->next;
    }
    pthread_mutex_unlock(&mtx_1);
    mark_on_heap(_metaData);
    sweep(_metaData);
    pthread_mutex_lock(&_SIGNAL_MUTEX);
    _CLEAN_FLAG = 0;
    pthread_cond_broadcast(&_SIGNAL_CV);
    pthread_mutex_unlock(&_SIGNAL_MUTEX);
    return NULL;
}

/*
 * should be called by one thread
 */
void gc_init()
{
    insertStackTopBottom();
    _metaData = DataStructure_init();
}

/*
 * should be called by one thread
 * initiate a thread-safe gc
 */
void gc_init_r(){
    _CLEAN_FLAG = 0;
    gc_init();
    signal(SIGUSR1, SIGNALHANDLER);
}


void insertStackTopBottom(){
    pthread_attr_t Attributes;
    void *StackAddress;
    size_t StackSize;

    // Get the pthread attributes
    memset (&Attributes, 0, sizeof (Attributes));
    pthread_getattr_np (pthread_self(), &Attributes);

    // From the attributes, get the stack info
    pthread_attr_getstack (&Attributes, &StackAddress, &StackSize);

    // Done with the attributes
    pthread_attr_destroy (&Attributes);

    size_t stack_top = (size_t)(StackAddress) + sizeof(size_t);
    size_t stack_bottom = (size_t)((void*)StackAddress + StackSize);
    pthread_mutex_lock(&ll_mtx);
    ll_insertNode(&pthread_ll_head, pthread_self(), stack_top, stack_bottom);
    pthread_mutex_unlock(&ll_mtx);

}

/*
 * should be called by one thread
 */
void gc_destroy()
{
    mark_all_on_stack(_metaData);
    sweep(_metaData);
    DataStructure_destroy(_metaData);
    ll_destroy(&pthread_ll_head);
}

void* gc_malloc(size_t size){
    pthread_mutex_lock(&_MALLOC_MUTEX);

    llNode* node = ll_findNode(pthread_ll_head, pthread_self()); 

    if(node == NULL){
        insertStackTopBottom();
    } else {
        int i = 0;
        node->stack_top = (size_t)(&i);
    }

    void* userData = malloc(size);
    if (userData == NULL)
        return NULL;

    clean_helper();
    Node_insert(_metaData, userData, size);	
    pthread_mutex_unlock(&_MALLOC_MUTEX);
    return userData;
}

void* gc_realloc(void* ptr, size_t size){
    if(ptr == NULL)
        return NULL;
    if(size == 0)
    {
        gc_free(ptr);
        return ptr;
    }
    Node* node = DataStructure_findNode(_metaData, ptr);
    size_t oldsize = node->size;
    size_t newsize = oldsize > size ? size : oldsize; 

    Node_remove(_metaData, ptr);

    void* newptr = realloc(ptr, size);
    if(newptr == NULL)
        return NULL;

    Node_insert(_metaData, newptr, newsize);

    return newptr;
}

void gc_free(void* ptr){
    if (ptr == NULL)
        return;
    else {
        Node* node = (Node*)DataStructure_findNode(_metaData, ptr);
        node->mark = 0;
    }
    return;
}

void* gc_calloc(size_t nmemb, size_t size){
    void* ptr = calloc(nmemb, size);
    if(ptr != NULL)
        Node_insert(_metaData, ptr, size);
    return ptr;
}
