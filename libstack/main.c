/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: Mon 02 Mar 2015 04:49:52 PM CST
 ************************************************************************/

#include <stdio.h>
#include "stack.h"

void stack_pop_view(stack_t * s)
{
    int * b;
    if(!stack_pop(s, (void **)&b))
    {
        printf("pop: %d\n",*b);
    }
    else
    {
        printf("stack empty\n");
    }
}
void stack_top_view(stack_t * s)
{
    int * b;
    if(!stack_top(s, (void **)&b))
    {
        printf("top: %d\n",*b);
    }
    else
    {
        printf("stack empty\n");
    }
}
int main(int argc, char ** argv)
{

    stack_t * s = stack_init();
    int a[10] = {1,2,3,4,5,6,7,8,9,10};
    
    stack_push(s, &a[0]);
    stack_push(s, &a[1]);
    stack_push(s, &a[2]);
    stack_push(s, &a[3]);
    stack_push(s, &a[4]);

    printf("stack size: %d\n", stack_size(s));
    stack_pop_view(s);
    stack_pop_view(s);
    stack_pop_view(s);
    stack_top_view(s);
    stack_top_view(s);
    printf("stack size: %d\n", stack_size(s));

    return 0;
}
