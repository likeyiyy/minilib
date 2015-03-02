# This is a -*- Makefile -*-

# Default target.
all:

CC = gcc
AR = ar

RM = rm -f
MKDIR = mkdir -p

INCLUDES =  
EXTRA_CFLAGS ?=

CFLAGS = -std=c99 -O2 -g -ffunction-sections -Wall -Wshadow -Werror \
  -D_GNU_SOURCE $(INCLUDES) $(EXTRA_CFLAGS)

DLL_FLAGS = \
  -Wl,--gc-sections \
  -Wl,--fatal-warnings \
  -Wl,--warn-unresolved-symbols


SOURCES := \
  queue.c \

-include $(SOURCES:.c=.d)


%.o: %.c
	$(CC) $(CFLAGS) -MD -MP -fpic -c -o $@ $<

libqueue.a: queue.o 
	rm -f $@
	$(AR) cq $@ $^

libqueue.so: queue.o 
	$(CC) $(CFLAGS) $(DLL_FLAGS) -Wl,-soname,$@ -shared -o $@ $^

main:main.c queue.c
	$(CC) -o $@ $^ -std=gnu99
		
all: libqueue.a libqueue.so main

clean:
	$(RM) *.d  *.o lib*.a lib*.so 

.PHONY: all clean

