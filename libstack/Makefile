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
  stack.c \

-include $(SOURCES:.c=.d)


%.o: %.c
	$(CC) $(CFLAGS) -MD -MP -fpic -c -o $@ $<

libstack.a: stack.o 
	rm -f $@
	$(AR) cq $@ $^

libstack.so: stack.o 
	$(CC) $(CFLAGS) $(DLL_FLAGS) -Wl,-soname,$@ -shared -o $@ $^

main:main.c stack.c
	$(CC) -o $@ $^ 
		
all: libstack.a libstack.so main

clean:
	$(RM) *.d  *.o lib*.a lib*.so 

.PHONY: all clean

