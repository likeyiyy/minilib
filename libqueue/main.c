/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: Mon 02 Mar 2015 05:59:30 PM CST
 ************************************************************************/

#include <stdio.h>
#include "queue.h"

void dequeue_view(queue_t * queue)
{
    int * b ;
    if(queue_dequeue(queue,(void **)&b) != 0)
    {
        printf("dequeue error\n");
    }
    else
    {
        printf("##################### dequeue : %d \n", *b);
    }

}
int main(int argc, char ** argv)
{
    queue_t * queue = queue_init(3 , TMC_QUEUE_SINGLE_SENDER | TMC_QUEUE_SINGLE_RECEIVER);

    int a [17];
    int result = -1;
    for(int i = 0; i < sizeof(a) / sizeof(a[0]); i++)
    {
        a[i] = i - 100;
        if(queue_enqueue(queue,&a[i]) != 0)
        {
            printf("enqueue error\n");
        }
    }
    printf("$$$$$$$$$$$$$$$$$$$ queue size : %d \n ", queue_size(queue));


    dequeue_view(queue);
    printf("$$$$$$$$$$$$$$$$$$$ queue size : %d \n ", queue_size(queue));
    dequeue_view(queue);
    printf("$$$$$$$$$$$$$$$$$$$ queue size : %d \n ", queue_size(queue));
    dequeue_view(queue);
    printf("$$$$$$$$$$$$$$$$$$$ queue size : %d \n ", queue_size(queue));
    dequeue_view(queue);
    printf("$$$$$$$$$$$$$$$$$$$ queue size : %d \n ", queue_size(queue));
    dequeue_view(queue);
    printf("$$$$$$$$$$$$$$$$$$$ queue size : %d \n ", queue_size(queue));
    dequeue_view(queue);
    printf("$$$$$$$$$$$$$$$$$$$ queue size : %d \n ", queue_size(queue));
    dequeue_view(queue);
    printf("$$$$$$$$$$$$$$$$$$$ queue size : %d \n ", queue_size(queue));
    dequeue_view(queue);
    printf("$$$$$$$$$$$$$$$$$$$ queue size : %d \n ", queue_size(queue));
    dequeue_view(queue);
    printf("$$$$$$$$$$$$$$$$$$$ queue size : %d \n ", queue_size(queue));

    
}

