
#ifndef _THRD_POOL_H
#define _THRD_POOL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>

//#define BEBUG

typedef void* (*pfun)(void *);
typedef struct ThrdPool ThrdPool;

/*	初始化
		回：该线程池结构
		参：任务池容量 最大线程个数 线程最大闲置 线程最小闲置 线程体函数
*/
ThrdPool* ThrdPool_init(int taskpoolnum, int maxNum, int maxIdleNum, int minIdleNum, void* (*func)(void*));

/*  任务添加
		回：0，添加正常	1，终止添加 2，已满
		参：任务入口参数
*/
int ThrdPool_addTask(ThrdPool* thrdpool, void *task);

/*	开始运行	死循环 任务到执行线程
*/
pthread_t ThrdPool_run(ThrdPool* thrdpool);


/*	数量管理	循环看数量 控制合理的数量
*/
pthread_t ThrdPool_NumCtl(ThrdPool* thrdpool);


/*	完工销毁			启动线程
*/
void ThrdPool_destroy(struct ThrdPool* thrdpool);

#ifdef __cplusplus
}
#endif

#endif
