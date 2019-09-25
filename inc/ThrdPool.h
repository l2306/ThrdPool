
#ifndef _THRD_POOL_H
#define _THRD_POOL_H

#ifdef __cplusplus
extern "C" {
#endif

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

//#define BEBUG

typedef void* (*pfun)(void *);
typedef struct ThrdPool ThrdPool;

/*	池信息初始化
		回：
		参：任务池容量 最大线程个数 线程最大闲置 线程最小闲置 线程体函数
*/
ThrdPool* ThrdPool_init(int taskpoolnum, int maxNum, int maxIdleNum, int minIdleNum, void* (*func)(void*));

/*	开启任务投递线程
		回：任务入pool投递线程id
		参：任务点投入taskpool的函数指针   注：此函数必须使用ThrdPool_addTaskToTaskpool进行投递
*/
pthread_t managerInterface(ThrdPool* thrdpool, void* (*inputTsk)(void*));

/*
		回：0，添加正常	1，终止添加 2，已满
		参：任务入口参数
*/
int ThrdPool_addTask(ThrdPool* thrdpool, void *task);

/*	程开工任务	死循环 任务到执行线程
*/
pthread_t ThrdPool_run(ThrdPool* thrdpool);
	static void managerMoveTaskToWorker(void* thrdpool);
	void manager_moveTaskToWorker(struct ThrdPool* thrdpool);

/*	线程数量管理	循环看数量 控制合理的数量
*/
pthread_t ThrdPool_NumCtl(ThrdPool* thrdpool);
	static void* managerThreadNum(void* thrdpool);

/*	完工销毁接口			启动线程
*/

void ThrdPool_destroy(struct ThrdPool* thrdpool);

#ifdef __cplusplus
}
#endif

#endif
