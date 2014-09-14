#include "threadpool.h"
#include <sys/sem.h>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cctype>

namespace mmtraining {

	/////////////////////////////////////////////////Thread


	Thread::Thread() : running(false), target(NULL), tid(-1) {}

	Thread::Thread(Runnable& t) : running(false), target(&t), tid(-1) {}

	Thread::~Thread() {
		// TODO: 释放资源
	}

	int Thread::Start() {
		// TODO: 启动线程, 运行 Run
		if ((pthread_create(&tid, NULL, start_thread, (void*)this)) != 0)
			return -1;
		else
			return 0;
	}

	pthread_t Thread::GetId() const {
		// TODO:获取线程id 
		if ((int)tid != -1)	
			return tid;
		else
			return -1;
	}

	int Thread::Run() {
		running = true;
		// 处理逻辑
		int ret = -1;

		if (target != NULL) { // 若指定了target, 则运行target逻辑
			ret = target->Run();
		} else { // 否则运行 DoRun 逻辑
			ret = this->DoRun();
		}
		running = false;
		return ret;
	}

	int Thread::DoRun() {
		printf("Thread %lld is running with no job in the class Thread.\n", (long long)pthread_self());
		return 0;
	}

	bool Thread::IsRunning() const {
		return running;
	}

	int Thread::Join() {
		// TODO: 完成代码
		if (pthread_join(tid, NULL) == 0)
		{
			return 0;
		}
		else
			return -1;
	}

	void* Thread::start_thread(void *arg)
	{
		Thread *ptr = (Thread*)arg;
		//sleep(0.01);
		ptr->Run();
		return NULL;
	}

	/////////////////////////////////////////////////ThreadPool

	ThreadPool::ThreadPool()
	{
	}

	ThreadPool::~ThreadPool() {
		// TODO: 完成代码
	}

	int ThreadPool::Start(int threadCount, Runnable& target) {
		// TODO: 完成代码
		for (int i=0; i<threadCount; i++)
		{
			Thread *newOneThread= new Thread(target);
			threads.push_back(newOneThread);
			if (newOneThread->Start() != 0)
				return -1;
		}
		return 0;
	}

	int ThreadPool::JoinAll() {
		// TODO: 完成代码
		for (Thread* &oneThread : threads)
		{
			if (oneThread->Join() != 0)
				return -1;	
			delete oneThread;
		}
		return 0;
	}

	int ThreadPool::IsThreadRunning()
	{
		// TODO: 方便工作池shutdown时的线程状态检查	
		bool allThreadFinish = true;
		for (Thread* &oneThread : threads)
		{
			if (oneThread->IsRunning())	
			{
				allThreadFinish = false;
				break;	
			}
		}
		return allThreadFinish;

	}

	///////////////////////////////////////////////WorkQueue

	WorkQueue::WorkQueue() : shutdown(false) {
		// TODO: 初始化
		if (pthread_mutex_init(&mutex, NULL) != 0)
		{
			printf("The pthread_mutex_init failed.");	
			exit(-1);
		}
		if (pthread_cond_init(&cond, NULL) != 0)
		{
			printf("The pthread_cond_init failed.");	
			exit(-1);
		}
	}

	WorkQueue::~WorkQueue() {
		// TODO: 释放资源
	}

	int WorkQueue::AddWork(Work* work) {
		// TODO: 完成代码
		pthread_mutex_lock(&mutex);
		works.push_back(work);
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
		return 0;
	}

	Work* WorkQueue::GetWork() {
		// TODO: 完成代码
		Work* oneWork = NULL; 
		pthread_mutex_lock(&mutex);
		while (!shutdown && works.empty())
			pthread_cond_wait(&cond, &mutex);
		if (!works.empty())	
		{
			oneWork = works.front();
			works.pop_front();
		}
		pthread_mutex_unlock(&mutex);
		if (oneWork != NULL)
			return (Work*)oneWork;
		else
			return NULL;
	}

	int WorkQueue::Shutdown() {
		// TODO: 完成代码
		int shutdownMask = 0;
		//char shutdownOK;
		shutdown = true;
		pthread_cond_broadcast(&cond);
		if (!works.empty())
			shutdownMask += 2;
		//for (Thread* oneThread : threadpool)
		//{
			////pthread_cond_destroy(&cond);
			//if (oneThread->IsRunning())
			//{
				//printf("ThreadPool is working, will you still shut it down?(Y/N)");	
				//scanf("%c", &shutdown);
				//if (toupper(shutdownOK) == 'Y')
				//{
					//exit(-1);	
				//}
				//else 
				//{
					//bool allThreadFinish = false;
					//while(!allThreadFinish)		
					//{
						//allThreadFinish = true;
						//for (Thread* insideOneThread : threadpool)
						//{
							//if (insideOneThread -> IsRunning())	
							//{
								//allThreadFinish = false;			
								//break;
							//}
						//}
					//}
					//shutdownMask += 1;
				//}
			//}
		//}
		
		return shutdownMask;
	}

	bool WorkQueue::IsShutdown() {
		return shutdown;
	}

	/////////////////////////////////////////////////Worker

	Worker::Worker(WorkQueue& queue) : workQueue(queue) {
		if (pthread_mutex_init(&mutex, NULL) == -1)	
		{
			printf("pthread_mutex_init failed.\n");	
			exit(-1);
		}
	}

	Worker::~Worker() {
		// TODO: 释放资源
	}

	int Worker::Run() {
		// TODO: 工作循环
		//printf("Thread %lld do this.\n", (long long)pthread_self());
		//printf("workQueue.IsShutdown() is %d\n", workQueue.IsShutdown());
		while (!workQueue.IsShutdown())
		{
			pthread_mutex_lock(&mutex);
			//printf("GetWork in worker.\n");
			Work* oneWork = workQueue.GetWork();
			pthread_mutex_unlock(&mutex);
			if (oneWork != NULL)
			{
				if (!oneWork->NeedDelete())
				{
					if(oneWork->DoWork() != 0)
					{
						// Dowork fails.
						//delete oneWork;
						return -1;
					}
					//else
						// Dowork successful.
						//delete oneWork;
				}
				//else
					//delete oneWork;
			}
		}
		pthread_exit(NULL);
		return 0;
	}

	/////////////////////////////////////////////////WorkerThreadPool

	WorkerThreadPool::WorkerThreadPool() : worker(workQueue) {}

	WorkerThreadPool::~WorkerThreadPool() {
		// TODO: 完成代码
	}

	int WorkerThreadPool::Start(int threadCount) {
		return pool.Start(threadCount, worker);
	}

	int WorkerThreadPool::Start(int threadCount, Runnable& target) {
		return pool.Start(threadCount, target);
	}

	int WorkerThreadPool::AddWork(Work* work) {
		return workQueue.AddWork(work);
	}

	int WorkerThreadPool::Shutdown() {
		return workQueue.Shutdown();
	}

	int WorkerThreadPool::JoinAll() {
		return pool.JoinAll();
	}

} // mmtrainin

