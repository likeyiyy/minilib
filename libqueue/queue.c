/*************************************************************************
	> File Name: queue.c
	> Author: 
	> Mail: 
	> Created Time: Mon 02 Mar 2015 05:33:07 PM CST
 ************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>
#include <unistd.h>
#include <strings.h>
#include <pthread.h>
#include "queue.h"


static inline void exit_if_ptr_is_null(void * p, char * des)
{
    if(p == NULL)
    {
        printf("%s\n",des);
        exit(-1);
    }
}
struct __queue
{
    uint64_t enqueue_count;
    uint64_t dequeue_count;
    void ** node;
    int item_size;
    uint32_t flags;
    unsigned int total;
    unsigned int length;
    pthread_mutex_t enqueue_mutex;
    pthread_mutex_t dequeue_mutex;
};

queue_t * queue_init(int log2_entries, uint32_t flags)
{
    queue_t * queue  =  memalign(64,sizeof(queue_t));
    unsigned int numbers = (1 << log2_entries);
    queue->node = malloc(numbers * sizeof(void *));
    exit_if_ptr_is_null(queue->node,"alloc queue node error");
    bzero(queue->node,numbers);
    queue->item_size = sizeof(void *);
    queue->total = numbers;
    queue->length = 0;
    queue->flags  = flags;
    queue->dequeue_count = 0;
    queue->enqueue_count = 0;
    pthread_mutex_init(&queue->enqueue_mutex,NULL);
    pthread_mutex_init(&queue->dequeue_mutex,NULL);
    return queue;
}
int queue_enqueue(queue_t * queue,
                  void * data)
{
    int result = -1;
    /* 为真则 if 为假*/
    if (!(queue->flags & TMC_QUEUE_SINGLE_SENDER)) 
    {
        pthread_mutex_lock(&queue->enqueue_mutex);
    }

    if(queue->enqueue_count - queue->dequeue_count >= (queue->total))
    {
        result = -1;
    }
    else
    {
        unsigned int i = queue->enqueue_count & ((queue->total) - 1);
        queue->node[i] = data;
        ++queue->enqueue_count;
        ++queue->length;
        result = 0;
    }
    if (!(queue->flags & TMC_QUEUE_SINGLE_SENDER)) 
    {
        pthread_mutex_unlock(&queue->enqueue_mutex);
    }
    return result;
}
int queue_enqueue_multiple(queue_t * queue,
                           void ** data,
                           unsigned int enqueue_num)
{
    int result = -1;
    if (!(queue->flags & TMC_QUEUE_SINGLE_SENDER)) 
    {
        pthread_mutex_lock(&queue->enqueue_mutex);
    }
    if(queue->enqueue_count + enqueue_num - queue->dequeue_count > (queue->total))
    {
        result = -1;
    }
    else
    {
        unsigned int enqueue_count = queue->enqueue_count;
        while(enqueue_num--)
        {
            unsigned int i = enqueue_count & ((queue->total) - 1);
            queue->node[i] = *(data++);
            ++queue->length;
            ++enqueue_count;
        }
        queue->enqueue_count = enqueue_count;
        result = 0;
    }
    if (!(queue->flags & TMC_QUEUE_SINGLE_SENDER)) 
    {
        pthread_mutex_unlock(&queue->enqueue_mutex);
    }
    return result;
}
/*
*  本函数不用is_empty_queue，因为防止锁中锁
* */
int queue_dequeue(queue_t * queue,
                  void ** data)
{
    int result = -1;
    if (!(queue->flags & TMC_QUEUE_SINGLE_RECEIVER))
    {
        pthread_mutex_lock(&queue->dequeue_mutex);
    }
    if(queue->enqueue_count == queue->dequeue_count)
    {
        result =  -1;
    }
    else
    {
        unsigned int i = queue->dequeue_count & ((queue->total) - 1);
        *data = queue->node[i];
        ++queue->dequeue_count;
        --queue->length;
        result = 0;
    }
    if (!(queue->flags & TMC_QUEUE_SINGLE_RECEIVER))
    {
        pthread_mutex_unlock(&queue->dequeue_mutex);
    }
    return result;
}

int queue_dequeue_multiple(queue_t * queue,
                           void ** data,
                           unsigned int dequeue_num)
{
    int result = -1;
    if (!(queue->flags & TMC_QUEUE_SINGLE_RECEIVER))
    {
        pthread_mutex_lock(&queue->dequeue_mutex);
    }
    if(queue->dequeue_count + dequeue_num  > queue->enqueue_count)
    {
        result = -1;
    }
    else
    {
        unsigned int dequeue_count = queue->dequeue_count;
        while(dequeue_num--)
        {
            unsigned int i = dequeue_count & ((queue->total) - 1);
            *(data++) = queue->node[i];
            --queue -> length;
            dequeue_count++;
        }
        queue->dequeue_count = dequeue_count;
        result = 0;
    }
    if (!(queue->flags & TMC_QUEUE_SINGLE_RECEIVER))
    {
        pthread_mutex_unlock(&queue->dequeue_mutex);
    }
    return 0;
}
int queue_empty(queue_t * queue)
{
    return queue->enqueue_count == queue->dequeue_count;
}
int queue_full(queue_t * queue)
{
    return queue->enqueue_count - queue->dequeue_count >= (queue->total);
}
int queue_size(queue_t * queue)
{
    return queue->length;
}
