/*
 * malloc.h
 *
 * Header file for malloc functions
 *
 */
#ifndef _MALLOC_H_
#define _MALLOC_H_

#include <stddef.h>
#include <pthread.h>

#define malloc(size) gc_malloc(size)
#define realloc(ptr, size) gc_realloc(ptr, size)
#define calloc(nmemb, size) gc_calloc(nmemb, size)
#define free(ptr) gc_free(ptr)
#define pthread_join(thread, retval) gc_pthread_join(thread, retval)

int _CLEAN_FLAG;
void SIGNALHANDLER();

void gc_init();
void gc_init_r();
void gc_destroy();
void* gc_malloc(size_t size);
void* gc_realloc(void* ptr, size_t size);
void gc_free(void* ptr);
void* gc_calloc(size_t nmemb, size_t size);
void gc_destroy();
void insertStackTopBottom();
int gc_pthread_join(pthread_t thread, void** retval);

#endif
