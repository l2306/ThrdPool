//www.cnblogs.com/zhiranok/archive/2013/01/14/task_queue.html

#ifndef _TASK_BINDER_
#define _TASK_BINDER_

#include <string>
#include <iostream>
#include <stdio.h>

using namespace std;

typedef void (*task_fun_t )(void *);

//任务接口
class task_impl_i
{
public:
    virtual ~task_impl_i(){}				
    virtual void run()          = 0;
    virtual task_impl_i* fork() = 0;
};

//实现	这里将任务定义为组合了数据和操作的对象
class task_impl_t: public task_impl_i
{
public:
	task_impl_t(task_fun_t fun_, void* arg_):
	        m_fun(fun_), m_arg(arg_)
	{}

	virtual void run(){
        	m_fun(m_arg);
	}

	virtual task_impl_i* fork(){
        	return new task_impl_t(m_fun, m_arg);
	}

protected:
	task_fun_t	m_fun;
	void*		m_arg;
};

//通用任务	为调用方便	实现多种方法
struct task_t
{
	//默认构造任务时  无函数定义时     防止run中fun奔溃
 	static void dumy(void*){}	

	task_t(task_fun_t f_, void* d_):
		task_impl(new task_impl_t(f_, d_))
	{}

	task_t(task_impl_i* task_imp_):
		task_impl(task_imp_)
	{}

	task_t(const task_t& src_):
		task_impl(src_.task_impl->fork())
	{}

	task_t(){
		task_impl = new task_impl_t(&task_t::dumy, NULL);
	}

	~task_t(){
		delete task_impl;
	}

    task_t& operator=(const task_t& src_){
        delete task_impl;
        task_impl = src_.task_impl->fork();
        return *this;
	}
    
	void run(){
		task_impl->run();
	}

    task_impl_i*    task_impl;
};

//将函数  和参数  封装为  task_t
struct task_binder
{
	//! C funtion
    
	static task_t gen(void (*fun_)(void*), void* p_)
	{
		return task_t(fun_, p_);
	}

	template<typename RET>
	static task_t gen(RET (*fun_)(void))
	{
		struct lambda_t
		{
			static void task_fun(void* p_)
			{
				(*(RET(*)(void))p_)();
			};
		};
		return task_t(lambda_t::task_fun, (void*)fun_);
	}

	template<typename FUN, typename ARG1>
	static task_t gen(FUN fun_, ARG1 arg1_)
	{
		struct lambda_t: public task_impl_i
		{
			FUN dst_fun;
			ARG1  arg1;
			lambda_t(FUN fun_, const ARG1& arg1_):
				dst_fun(fun_), arg1(arg1_)
			{}
			virtual void run()
			{
				(*dst_fun)(arg1);
			}
			virtual task_impl_i* fork()
			{
				return new lambda_t(dst_fun, arg1);
			}
		};
		return task_t(new lambda_t(fun_, arg1_));
	}	
	
	template<typename FUN, typename ARG1>
	static task_t* gen_ptr(FUN fun_, ARG1 arg1_)
	{
		struct lambda_t: public task_impl_i
		{
			FUN dst_fun;
			ARG1  arg1;
			lambda_t(FUN fun_, const ARG1& arg1_):
				dst_fun(fun_), arg1(arg1_)
			{}
			virtual void run()
			{
				(*dst_fun)(arg1);
			}
			virtual task_impl_i* fork()
			{
				return new lambda_t(dst_fun, arg1);
			}
		};
		return new task_t(new lambda_t(fun_, arg1_));
	}	
};

void prt(void* p)
{
	printf("%s \n",(char*)p);
}

//
typedef struct{
	int id;
	int pswd;
} Data_1;
typedef void (*Func1)(Data_1* typ);
void fun_tsk1(Data_1* typ)
{
	cout<<"Data_1 " << typ->id << " " << typ->pswd << endl;
}

int test()
{
	char cs[127]={"1234567"};
	task_t tsk = task_binder::gen(prt, cs);
	tsk.run();
	
	Data_1 data1 = {11,22 };
	task_t  t1=task_binder::gen<Func1,Data_1*>(fun_tsk1, &data1);
	t1.run();

	return 0;
}

#endif
