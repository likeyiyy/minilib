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
  list.c \

-include $(SOURCES:.c=.d)


%.o: %.c
	$(CC) $(CFLAGS) -MD -MP -fpic -c -o $@ $<

liblist.a: list.o 
	rm -f $@
	$(AR) cq $@ $^

liblist.so: list.o 
	$(CC) $(CFLAGS) $(DLL_FLAGS) -Wl,-soname,$@ -shared -o $@ $^
		
all: liblist.a liblist.so 

clean:
	$(RM) *.d  *.o lib*.a lib*.so 

.PHONY: all clean

