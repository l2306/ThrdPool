#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<unistd.h>

#include "../inc/ThrdPool.h"
#include "task_binder.h"

static int j=0;

void* th_fun(void *task)         
{
++j;
	task_t* t2=(task_t*)task;
	t2->run();
    delete t2;
	return NULL;
}


int main ( int argc, char * argv[] )
{
	ThrdPool* thrdpool = ThrdPool_init(10000, 10, 4, 2, (pfun)th_fun);
	printf("managerInit \n");

//	printf("inputTsk %x\n", managerInterface(thrdpool, inputTsk));
	Data_1 tsks[10000];
	for(int i=0;i<10000;i++)
	{	
		tsks[i].id=i;
		tsks[i].pswd=100*i;
		task_t*  t1=task_binder::gen_ptr<Func1,Data_1*>(fun_tsk1, &tsks[i]);
		//usleep(1);
		ThrdPool_addTask(thrdpool ,(void*)t1);
	}

	printf("mng_tsk2work %x\n", ThrdPool_run(thrdpool));

	printf("mng_thrNumCtrl %x\n", ThrdPool_NumCtl(thrdpool));

//	inputTsk(thrdpool);


	sleep(5);
	printf("----%d----",j);
	ThrdPool_destroy(thrdpool);

	return(0);
}

/*
查看内存泄漏
	g++ *.cc -g -o t2 -lpthread -rdynamic
	valgrind --leak-check=yes ./t2
*/

