/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: Wed 26 Nov 2014 11:20:49 AM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

int compare_int(void * a, void * b)
{
    int * a1 = (int *)a;
    int * a2 = (int *)b;
    if( *a1 == *a2 )
    {
        return 0;
    }
    return 1;
}

int view_int(void * a)
{
    int * a1 = (int *)a;
    printf("->[%d]",*a1);
}
int main()
{
    int a[11];
    struct blist node[11]; 
    struct list_head * intlist = list_init(compare_int,view_int);
    for(int i = 0; i < 10; i++)
    {
        a[i] = i ;
        node[i].item = &a[i];
        list_add(&node[i].listhead, intlist);
    }
    list_view(intlist);

    int c = 6;
    int j = 10;
    int k = 22;
    list_add_before_item(intlist,&c,&j);
    list_add_after_item(intlist,&c,&k);

    list_view(intlist);

    list_del_item(intlist, &c);

    list_view(intlist);

    int m = 88;
    list_modify_item(intlist, &j, &k);

    list_view(intlist);

}
