/*************************************************************************
	> File Name: stack.c
	> Author: 
	> Mail: 
	> Created Time: Mon 02 Mar 2015 04:20:21 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "stack.h"

struct list_head {
	struct list_head* next; 
	struct list_head* prev;
    uint64_t count;
};

struct blist
{
    void * item;
	struct list_head listhead;
};

void INIT_LIST_HEAD(struct list_head *list)
{
	list->next = list;
	list->prev = list;
}

static inline void __list_add(struct list_head *new,
			      			  struct list_head *prev,
			      			  struct list_head *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

void list_add(struct list_head *new, struct list_head *head)
{
	__list_add(new, head, head->next);
}

void list_add_tail(struct list_head *new, struct list_head *head)
{
	__list_add(new, head->prev, head);
}

static inline void __list_del(struct list_head * prev, struct list_head * next)
{
	next->prev = prev;
	prev->next = next;
}

void list_del(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	entry->next = NULL;
	entry->prev = NULL;
}

void list_move_tail(struct list_head *list,
				  	struct list_head *head)
{
	__list_del(list->prev, list->next);
	list_add_tail(list, head);
}

static inline int list_empty(const struct list_head *head)
{
	return head->next == head;
}

#define list_entry(ptr, type, member) \
	((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))
	
#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); \
        	pos = pos->next)

#define list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
			pos = n, n = pos->next)

stack_t * stack_init()
{
    stack_t * stack = malloc(sizeof(stack_t));
    INIT_LIST_HEAD(stack);
    stack->count = 0;
    return stack;
}

int stack_pop(stack_t * s,void ** item)
{
    if(list_empty(s) == true)
    {
        return -1;
    }
    struct list_head * list_tail = s->prev;
    list_del(list_tail);
    struct blist * node = list_entry(list_tail,struct blist,listhead);
    *item = node->item;
    s->count--;
    return 0;
}
int stack_top(stack_t * s,void ** item)
{
    if(list_empty(s) == true)
    {
        return -1;
    }
    struct list_head * list_tail = s->prev;
    struct blist * node = list_entry(list_tail,struct blist,listhead);
    *item = node->item;
    return 0;
}
int stack_push(stack_t * s, void * item)
{
    struct blist * new = malloc(sizeof(struct blist));
    if(new == NULL)
    {
        return -1;
    }
    new->item = item;
    list_add_tail(&new->listhead,s);
    s->count++;
    return 0;
}
int stack_size(stack_t * s)
{
    return s->count;
}
int stack_empty(stack_t * s)
{
    return list_empty(s);
}
