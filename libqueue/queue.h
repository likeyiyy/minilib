#ifndef __QUEUE_INTEL_H__
#define __QUEUE_INTEL_H__

#define TMC_QUEUE_NO_FLAGS          (0)
#define TMC_QUEUE_SINGLE_SENDER     (1 << 0)
#define TMC_QUEUE_SINGLE_RECEIVER   (1 << 1)

#include <stdint.h>
typedef struct __queue queue_t;                                                                                          

queue_t * queue_init(int log2_entries,
                    uint32_t flags);
int queue_enqueue(queue_t * queue,
                  void * data);
int queue_enqueue_multiple(queue_t * queue,
                           void ** data,
                           unsigned int enqueue_num);
int queue_dequeue(queue_t * queue,
                  void ** data);
                                                                                                    
int queue_dequeue_multiple(queue_t * queue,
                           void ** data,
                           unsigned int dequeue_num);
int queue_empty(queue_t * queue);
int queue_full(queue_t * queue);
int queue_size(queue_t * queue);
#endif
