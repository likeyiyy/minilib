/*************************************************************************
	> File Name: stack.h
	> Author: 
	> Mail: 
	> Created Time: Mon 02 Mar 2015 04:01:26 PM CST
 ************************************************************************/

#ifndef _STACK_H
#define _STACK_H
typedef struct list_head stack_t;
stack_t * stack_init();
int stack_pop(stack_t * s,void ** item);
int stack_top(stack_t * s,void ** item);
int stack_push(stack_t * s, void * item);
int stack_size(stack_t * s);
int stack_empty(stack_t * s);
#endif
