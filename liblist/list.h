/*************************************************************************
	> File Name: list.h
	> Author: likeyi
	> Mail: likeyiyy@sina.com 
	> Created Time: Wed 11 Jun 2014 10:32:56 AM CST
 ************************************************************************/


#ifndef __LIST_H_
#define __LIST_H_

typedef int (*compare_item_fp)(void * a,void *b);
typedef int (*view_item_fp)(void * a);

struct list_head {
	struct list_head* next; 
	struct list_head* prev;
    int (* compare_item)(void * listitem, void * item);
    int (* view_item)(void * listitem);
};

struct blist
{
    void * item;
	struct list_head listhead;
};
/**
* Low leave API
* */
void list_add(struct list_head *new, 
              struct list_head *head);
void list_add_tail(struct list_head *new, 
                   struct list_head *head);
void list_del(struct list_head *entry);
void list_move_tail(struct list_head *list,
				  	struct list_head *head);
/**
* High leave API
* */
struct list_head * list_init(compare_item_fp compare_fp, view_item_fp view_fp);
void list_view(struct list_head * head);
int  list_empty(const struct list_head *head);
/*
* 【后增】存在item插入成功，不存在，插入失败。
* */
int list_add_after_item(struct list_head * head, 
                        void * item, 
                        void * adding);
/*
* 【前增】存在item插入成功，不存在，插入失败。
* */
int list_add_before_item(struct list_head * head, 
                        void * item, 
                        void * adding);
/*
* 【删】存在item删除成功，不存在，删除失败。
* */
int list_del_item(struct list_head * head, 
                  void * item);
/*
* 【改】存在item修改成功，不存在，修改失败。
* */
int list_modify_item(struct list_head * head,
                     void * item, 
                     void * modifying);
/*
* 【查】存在item返回，不存在，返回NULL。
* */
struct blist * list_find(struct list_head * head, void * item);

#endif /* __LIST_H_ */
