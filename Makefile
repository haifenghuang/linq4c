cc=/usr/local/bin/gcc

SRCS=ArrayList.c linq.c hashmap.c main.c ./gc4c/dataStructure.c ./gc4c/linkedList.c ./gc4c/malloc.c ./gc4c/mark_and_sweep.c
MACRO_SRCS=ArrayList.c linq.c hashmap.c main_macro.c ./gc4c/dataStructure.c ./gc4c/linkedList.c ./gc4c/malloc.c ./gc4c/mark_and_sweep.c

DEBUG_FLAG=-g
INCLUDE=-I. -I./gc4c
CFLAGS=-D_GNU_SOURCE $(DEBUG_FLAG) $(INCLUDE)
LD_FLAGS=-lpthread 

TARGET_LINQ=linq
TARGET_LINQ_MACRO=linq_macro

all:
	$(cc) $(CFLAGS) $(SRCS) $(LD_FLAGS) -o $(TARGET_LINQ)
	$(cc) $(CFLAGS) $(MACRO_SRCS) $(LD_FLAGS) -o $(TARGET_LINQ_MACRO)

clean:
	rm -f $(TARGET_LINQ) $(TARGET_LINQ_MACRO)
