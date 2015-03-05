/*************************************************************************
	> File Name: hash.c
	> Author: likeyi
	> Mail: likeyi@sina.com 
	> Created Time: Tue 17 Jun 2014 04:36:47 PM CST
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include "hash.h"
static inline void exit_if_ptr_is_null(void * p, char * des)
{
    if(p == NULL)
    {
        printf("%s\n",des);
        exit(-1);
    }
}
struct list_head {
	struct list_head* next; 
	struct list_head* prev;
};
struct blist
{
	void * item;
	struct list_head listhead;
};
static inline void INIT_LIST_HEAD(struct list_head *list)
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
static inline void list_add(struct list_head *new, struct list_head *head)
{
	__list_add(new, head, head->next);
}
static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
	__list_add(new, head->prev, head);
}
static inline void __list_del(struct list_head * prev, struct list_head * next)
{
	next->prev = prev;
	prev->next = next;
}
static inline void list_del(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	entry->next = NULL;
	entry->prev = NULL;
}
static inline void list_move_tail(struct list_head *list,
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

struct _bucket { 
    struct list_head list;
    pthread_mutex_t  lock;
    uint32_t count;
}; 
struct _hash_table {
    int num_buckets;
    compare_item_fp compare_item;
    hash_key_fp     hash_key;
    hash_found_fp   item_not_found;
    hash_found_fp   item_found;
    bucket_t *buckets;
};




static struct blist * find_list(hash_table * ht, 
                                struct list_head * head, 
                                void * item)
{
    struct list_head * p;
    struct blist * node;
    exit_if_ptr_is_null(ht->compare_item,"table->compare_item == NULL");
    list_for_each(p,head)
    {
        node = list_entry(p,struct blist,listhead);
        if(ht->compare_item(&node->item, item) == 0)
        {
            return node;
        }
    }
    return NULL;
}



static inline int next_prime(int x)
{
     long i, j;
     int f;
     x = (x==0)?1:x;
     i = x;
     while (i++)
     {
          f=1;
          for (j=2; j<i; j++)
          {
               if (i%j == 0)
               {
                    f=0;
                    break;
               }
          }
          if (f)
          {
             return (int)i;
          }
     }
    return 0;
}
/*
* 1. 创建
* */
hash_table * hash_create(int num,
                        compare_item_fp compare,
                        hash_key_fp hash_key)
{
    hash_table * ht;
    bucket_t * b;
    int bytes;
    int i;
    ht = malloc(sizeof(hash_table));
    exit_if_ptr_is_null(ht,"Initizial hash table Error"); 
    num = next_prime(num);
    bytes = num * sizeof(bucket_t);
    ht->buckets = b = malloc(bytes);
    exit_if_ptr_is_null(ht->buckets,"hash table buckets alloc error");
    ht->num_buckets = num;
    ht->compare_item = compare;
    ht->hash_key     = hash_key;
    i = num;
    while(--i >= 0)
    {
        INIT_LIST_HEAD(&b->list); 
        pthread_mutex_init(&b->lock, NULL);
        b->count = 0;
        b++;
    }
    return ht;
}
/*
* 2. 查找
* */
void * hash_lookup_item(hash_table * ht, void * value)
{
    struct list_head * ll;
    exit_if_ptr_is_null(ht->hash_key,"hash key is null");
    uint32_t key = ht->hash_key(value);
    bucket_t * bucket = &ht -> buckets[key % ht->num_buckets];
    ll = &bucket->list;
    return (void *) find_list(ht, ll, value);    
}
/*
* 3. 插入
* 严格说来,blist 完全没有任何问题。
* */
int  hash_add_item(hash_table * ht, void * value )
{
    struct list_head * ll;
    struct blist * blist;
    struct blist * new_blist;

    exit_if_ptr_is_null(ht->hash_key,"hash key is null");
    uint32_t key = ht->hash_key(value);
    bucket_t * bucket = &ht -> buckets[key % ht->num_buckets];
    pthread_mutex_lock(&bucket->lock);
    ll = &bucket->list;
    blist = find_list(ht, ll, value);    
    /*
     * 假如不存在于链表中。
     * */
    if(!blist)
    {
        ht->item_not_found(ht,ll,value);
    }
    /* 
    * Found it, and memcpy it.
    * 主要是这一段无法公共化，其他的hash函数可能找到后，并不会copy
    * */
    else
    {
        ht->item_found(ht,ll,value);
    }
    pthread_mutex_unlock(&bucket->lock);
    return 0;
}
/*
* 4. 遍历
* */
void  hash_travel_delete(hash_table * ht)
{
    register int i = 0;
    register bucket_t * bucket;
    bucket = ht->buckets;
    while(i++ < ht->num_buckets)
    {
        pthread_mutex_lock(&bucket->lock);
        delete_session(ht,bucket);
        pthread_mutex_unlock(&bucket->lock);
        bucket++;
    }
}
/*
 * unsigned int hash_count(hash_table *ht)
 *
 * Return total number of elements contained in hash table.
 */
uint32_t hash_count(hash_table * ht)
{
    register int i = 0;
    register int cnt = 0;
    register bucket_t *bucket;
    bucket = ht->buckets;
    while (i++ < ht->num_buckets)
    {
	    cnt += bucket->count;
	    bucket++;
    }
    return cnt;
}
