
//注：此list.h 是为配合示例程序而建的，内容来自：linux/include/linux/list.h 和相关文件
//仿照内核的list函数	linux内核提供用于创建双向循环链表的结构lst_node

#ifndef _LINUX_lst_H
#define _LINUX_lst_H
 
#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <unistd.h>	//sleep

/*
__函数名 为原始函数
*/

struct lst_node {
         struct lst_node *next, *prev;
};

#define lst_HEAD_INIT(name) { &(name), &(name) }

//type结构中member成员的偏移量
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)	

//根据type结构体中member成员的指针ptr 求出所在的结构体的指针
#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})

//创建双向链表
static inline void INIT_lst_HEAD(struct lst_node *list)
{
        list->next = list;
        list->prev = list;
}

//基础函数  将 newnode 添加到 prev 和 next 之间
static inline void __lst_add(struct lst_node *newnode, struct lst_node *prev,struct lst_node *next)
{
        next->prev = newnode;
        newnode->next = next;		//为防止编译器和cpu优化代码的执行顺序,,可在这里添加 smp_wmb();
        newnode->prev = prev;
        prev->next = newnode;
}

//将new添加到head后
static inline void lst_add(struct lst_node *newnode, struct lst_node *head)
{
        __lst_add(newnode, head, head->next);
}

//将new添加到 链表尾部
static inline void lst_add_tail(struct lst_node *newnode, struct lst_node *head)
{
        __lst_add(newnode, head->prev, head);
}

//基础函数  删除 prev 和 next 之间的 节点
static inline void __lst_del(struct lst_node * prev, struct lst_node * next)
{
        next->prev = prev;
        prev->next = next;
}
//删除 entry 节点
static inline void lst_del(struct lst_node* entry)		
{
        __lst_del(entry->prev, entry->next);
        entry->next = NULL;
        entry->prev = NULL;
}

//弹出 entry 节点
static inline struct lst_node* lst_pop(struct lst_node* entry)		
{
        __lst_del(entry->prev, entry->next);
        entry->next = NULL;
        entry->prev = NULL;
        return entry;
}

//检测 链表是否为空
static inline int lst_empty(const struct lst_node* head)
{
		return (head->next == head);
}

#define prefetch(x) __builtin_prefetch(x)		//注：这里prefetch 是gcc的一个优化，也可以不要

#define lst_for_each(pos, head) \
         for (pos = (head)->next; prefetch(pos->next), pos != (head);pos = pos->next)

#define lst_entry(ptr, type, member) \
         container_of(ptr, type, member)
#endif

#ifdef __cplusplus
}
#endif
