#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<unistd.h>

#include "../inc/ThrdPool.h"

typedef  struct  taskNode{
	int iTaskType;
	int sockfd;
}taskNode;


static int i=0;
//automic_t at=0;

void* th_fun(void *task)         
{
//RUNHERE;
	taskNode *node=(taskNode*)task;
	//usleep(1);
	switch (node->iTaskType)
	{
	case 0:
//		printf("--0--	work ptid = %x task->value = %d %d\n", (int)pthread_self(), node->sockfd, i++);
	sleep(1);
	printf("--0--	work ptid = %x task->value = %d %d\n", (int)pthread_self(), node->sockfd,i);
	sleep(1);
	printf("--0--	work ptid = %x task->value = %d %d\n", (int)pthread_self(), node->sockfd, i++);
		break;
	case 1:
		printf("--1--	work ptid = %x task->value = %d %d\n", (int)pthread_self(), node->sockfd, i++);
		break;
	case 2:
		printf("--2--	work ptid = %x task->value = %d %d\n", (int)pthread_self(), node->sockfd, i++);
		break;
	default:
		break;
	}
	free(task);
	return NULL;
}


int main ( int argc, char * argv[] )
{
	ThrdPool* thrdpool = ThrdPool_init(10000, 10, 4, 2, (pfun)th_fun);
	printf("managerInit \n");

	for(i=0;i<10000;i++)
	{	
		taskNode *task = (taskNode*)malloc(sizeof(taskNode));
		task->iTaskType = i%3;
		task->sockfd =i;
		//usleep(1);
		ThrdPool_addTask(thrdpool ,(void*)task);
	}

	printf("mng_tsk2work %x\n", ThrdPool_run(thrdpool));
	printf("mng_thrNumCtrl %x\n", ThrdPool_NumCtl(thrdpool));

	sleep(5);
	ThrdPool_destroy(thrdpool);

	exit(0);
}

/*
查看内存泄漏
	gcc *.c -g -o t1 -lpthread -rdynamic -lThrdPool
	valgrind --leak-check=yes ./t1
*/

