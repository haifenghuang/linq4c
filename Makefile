cc=/usr/local/bin/gcc

SRCS=ArrayList.c linq.c hashmap.c main.c 
MACRO_SRCS=ArrayList.c linq.c hashmap.c main_macro.c

DEBUG_FLAG=-g
INCLUDE=-I.
CFLAGS=-D_GNU_SOURCE $(DEBUG_FLAG) $(INCLUDE)
LD_FLAGS=-lpthread 

TARGET_LINQ=linq
TARGET_LINQ_MACRO=linq_macro

all:
	$(cc) $(CFLAGS) $(SRCS) $(LD_FLAGS) -o $(TARGET_LINQ) -lgc
	$(cc) $(CFLAGS) $(MACRO_SRCS) $(LD_FLAGS) -o $(TARGET_LINQ_MACRO) -lgc

clean:
	rm -f $(TARGET_LINQ) $(TARGET_LINQ_MACRO)
