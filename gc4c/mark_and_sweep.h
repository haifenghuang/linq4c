#ifndef _MARK_H_
#define _MARK_H_

#include <pthread.h>
#include "dataStructure.h"

static pthread_mutex_t _mark_sweep_mtx = PTHREAD_MUTEX_INITIALIZER;

void find_stack_bottom();
int mark(size_t sp, DataStructure *heapdata);
void mark_on_stack(DataStructure *heapdata, size_t, size_t, long);
void mark_on_heap(DataStructure *heapdata);
void mark_all_on_stack(DataStructure *heapdata);
void set_stack_bottom(size_t bottom);
void sweep(DataStructure *heapdata);
// void set_stack_top_and_bottom(size_t stack_top_input, size_t stack_bottom_input);
	
void mark_all_on_stack_r(DataStructure *heapdata);

#endif
