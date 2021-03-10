
//注：仿：linux/include/linux/list.h 内核
//	创建双向循环链表的结构list_node

#ifndef _LINUX_list_H
#define _LINUX_list_H
 
#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <unistd.h>

struct list_node {
         struct list_node *next, *prev;
};

// type结构中member成员的偏移量
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)	

// 根据type结构体中member成员的指针ptr 求出所在的结构体的指针
#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})

// 创建双向链表
static inline void INIT_list_HEAD(struct list_node *list)
{
        list->next = list;
        list->prev = list;
} //#define list_HEAD_INIT(name) { &(name), &(name) }

// 基础函数  将 newnode 添加到 prev 和 next 之间
static inline void __list_add(struct list_node *newnode, struct list_node *prev,struct list_node *next)
{
        next->prev = newnode;
        newnode->next = next;		//为防止编译器和cpu优化代码的执行顺序,,可在这里添加 smp_wmb();
        newnode->prev = prev;
        prev->next = newnode;
}

// 将new添加到head后
static inline void list_add(struct list_node *newnode, struct list_node *head)
{
        __list_add(newnode, head, head->next);
}

// 将new添加到 链表尾部
static inline void list_add_tail(struct list_node *newnode, struct list_node *head)
{
        __list_add(newnode, head->prev, head);
}

// 基础函数  删除 prev 和 next 之间的 节点
static inline void __list_del(struct list_node * prev, struct list_node * next)
{
        next->prev = prev;
        prev->next = next;
}
// 删除 entry 节点
static inline void list_del(struct list_node* entry)		
{
        __list_del(entry->prev, entry->next);
        entry->next = NULL;
        entry->prev = NULL;
}

// 弹出 entry 节点
static inline struct list_node* list_pop(struct list_node* entry)		
{
        __list_del(entry->prev, entry->next);
        entry->next = NULL;
        entry->prev = NULL;
        return entry;
}

// 检测 链表是否为空
static inline int list_empty(const struct list_node* head)
{
		return (head->next == head);
}

#define prefetch(x) __builtin_prefetch(x)		//注：这里prefetch 是gcc的一个优化，也可以不要

#define list_for_each(pos, head) \
         for (pos = (head)->next; prefetch(pos->next), pos != (head);pos = pos->next)

#define list_entry(ptr, type, member) \
         container_of(ptr, type, member)
#endif

#ifdef __cplusplus
}
#endif
