/*************************************************************************
	> File Name: list.c
	> Author: 
	> Mail: 
	> Created Time: Wed 26 Nov 2014 10:20:33 AM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

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

int list_empty(const struct list_head *head)
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

struct list_head * list_init(compare_item_fp compare_fp, view_item_fp view_fp)
{
    struct list_head * list = malloc(sizeof(struct list_head));
    INIT_LIST_HEAD(list);
    list->compare_item = compare_fp;
    list->view_item    = view_fp;
    return list;
}
/*
* 【查】存在item返回，不存在，返回NULL。
* */
struct blist * list_find(struct list_head * head, void * item)
{
    struct list_head * p;
    struct blist * node;
    if(head->compare_item == NULL)
    {
        printf("compare_item is NULL\n");
        exit(-1);
    }
    list_for_each(p,head)
    {   
        node = list_entry(p,struct blist,listhead);
        if(head->compare_item(node->item,item) == 0)
        {
            return node;
        }
    }
    return NULL;
}

void list_view(struct list_head * head)
{
    struct list_head * p;
    struct blist * node;
    if(head->view_item == NULL)
    {
        printf("view_item is NULL\n");
        exit(-1);
    }
    list_for_each(p,head)
    {   
        node = list_entry(p,struct blist,listhead);
        head->view_item(node->item);
    }
    printf("\n");
}
/*
* 【后增】存在item插入成功，不存在，插入失败。
* */
int list_add_after_item(struct list_head * head, 
                        void * item, 
                        void * adding)
{
    struct blist * b = list_find(head,item);
    if(b == NULL)
    {
        return -1;
    }
    struct blist * new = malloc(sizeof(struct blist));
    new->item = adding;
    list_add(&new->listhead,&b->listhead);
    return 0;
}
/*
* 【前增】存在item插入成功，不存在，插入失败。
* */
int list_add_before_item(struct list_head * head, 
                        void * item, 
                        void * adding)
{
    struct blist * b = list_find(head,item);
    if(b == NULL)
    {
        return -1;
    }
    struct list_head * list = &b->listhead;
    list = list->prev;
    struct blist * new = malloc(sizeof(struct blist));
    new->item = adding;
    list_add(&new->listhead,list);
    return 0;
}
/*
* 【删】存在item删除成功，不存在，删除失败。
* */
int list_del_item(struct list_head * head, 
                  void * item)
{
    struct blist * b = list_find(head,item);
    if(b == NULL)
    {
        return -1;
    }
    list_del(&b->listhead);
    return 0;
}
/*
* 【改】存在item修改成功，不存在，修改失败。
* */
int list_modify_item(struct list_head * head,
                     void * item, 
                     void * modifying)
{
    struct blist * b = list_find(head,item);
    if(b == NULL)
    {
        return -1;
    }
    b->item = modifying;
    return 0;
}
