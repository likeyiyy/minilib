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
        bucket_t *buckets;
};




struct blist * find_list(struct list_head * head, void * item)
{
    struct list_head * p;
    struct blist * node;
    list_for_each(p,head)
    {
        node = list_entry(p,struct blist,listhead);
        if(compare_session(&node->item, flow) == 0)
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
hash_table * hash_create(int num)
{
    hash_table * result;
    bucket_t * b;
    int bytes;
    int i;
    result = malloc(sizeof(hash_table));
    exit_if_ptr_is_null(result,"Initizial hash table Error"); 
    num = next_prime(num);
    bytes = num * sizeof(bucket_t);
    result->buckets = b = malloc(bytes);
    exit_if_ptr_is_null(result->buckets,"hash table buckets alloc error");
    result->num_buckets = num;
    i = num;
    while(--i >= 0)
    {
        INIT_LIST_HEAD(&b->list); 
        pthread_mutex_init(&b->lock, NULL);
        b->count = 0;
        b++;
    }
    return result;
}
/*
* 2. 查找
* */
void * hash_lookup_item(hash_table * ht, uint32_t key, void * value)
{
    struct blist * list = (struct blist *)value;
    struct list_head * ll;
    session_item_t * session = &list->item;
    bucket_t * bucket = &ht -> buckets[key % ht->num_buckets];
    ll = &bucket->list;
    return (void *) find_list(ll,(flow_item_t *)session);    
}
static inline void make_new_session(struct blist * blist,
                                    flow_item_t * flow,
                                    manager_t * manager)
{
    session_item_t * item = &blist->item;
    item->pool       = manager->session_pool;
    item->length     = global_config->manager_buffer_size;
    item->cur_len    = 0;
    item->upper_ip   = flow->upper_ip;
    item->lower_ip   = flow->lower_ip;
    item->upper_port = flow->upper_port;
    item->lower_port = flow->lower_port;
    item->protocol   = flow->protocol;
    item->last_time  = GET_CYCLE_COUNT();
    memcpy(item->buffer,flow->payload,flow->payload_len);
    item->cur_len  += flow->payload_len;
}
/*
* 3. 插入
* 严格说来,blist 完全没有任何问题。
* */
int  hash_add_item(hash_table ** htp, uint32_t key, void * value )
{
    struct list_head * ll;
    struct blist * blist;
    struct blist * new_blist;
    hash_table * ht = *htp;
    //flow_item_t * flow = (flow_item_t *)value;
    //manager_t * manager = list_entry(htp,manager_t,ht);
    bucket_t * bucket = &ht -> buckets[key % ht->num_buckets];
    pthread_mutex_lock(&bucket->lock);
    ll = &bucket->list;
    blist = find_list(ll,flow);    
    /*
     * 假如不存在于链表中。
     * */
    if(!blist)
    {
    }
    /* 
    * Found it, and memcpy it.
    * 主要是这一段无法公共化，其他的hash函数可能找到后，并不会copy
    * */
    else
    {

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
