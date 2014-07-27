#include "threadpool.h"
#include <iostream>
#include <pthread.h>
#include <unistd.h>

namespace mmtraining {

	/////////////////////////////////////////////////Thread


	Thread::Thread() : running(false), target(NULL), tid(-1) {}

	Thread::Thread(Runnable& t) : running(false), target(&t), tid(-1) {}

	Thread::~Thread() {
		// TODO: �ͷ���Դ
	}

	int Thread::Start() {
		// TODO: �����߳�, ���� Run
		if ((pthread_create(&tid, NULL, start_thread, (void*)this)) != 0)
			return -1;
		else
			return 0;
	}

	pthread_t Thread::GetId() const {
		// TODO:��ȡ�߳�id 
		if ((int)tid != -1)	
			return tid;
		else
			return -1;
	}

	int Thread::Run() {
		running = true;
		// �����߼�
		int ret = -1;

		if (target != NULL) { // ��ָ����target, ������target�߼�
			ret = target->Run();
		} else { // �������� DoRun �߼�
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
		// TODO: ��ɴ���
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
		// TODO: ��ɴ���
	}

	int ThreadPool::Start(int threadCount, Runnable& target) {
		// TODO: ��ɴ���
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
		// TODO: ��ɴ���
		for (Thread* &oneThread : threads)
		{
			if (oneThread->Join() != 0)
				return -1;	
		}
		return 0;
	}

	///////////////////////////////////////////////WorkQueue

	WorkQueue::WorkQueue() : shutdown(false) {
		// TODO: ��ʼ��
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
		// TODO: �ͷ���Դ
	}

	int WorkQueue::AddWork(Work* work) {
		// TODO: ��ɴ���
		works.push_back(work);
		pthread_cond_signal(&cond);
		return 0;
	}

	Work* WorkQueue::GetWork() {
		// TODO: ��ɴ���
		Work* oneWork = NULL; 
		pthread_mutex_lock(&mutex);
		if (!shutdown)
		{
			if (works.empty())
				pthread_cond_wait(&cond, &mutex);
			else
			{
				oneWork = works.front();
				works.pop_front();
			}
		}
		pthread_mutex_unlock(&mutex);
		if (oneWork != NULL)
			return (Work*)oneWork;
		else
			return NULL;
	}

	int WorkQueue::Shutdown() {
		// TODO: ��ɴ���
		printf("Shutdown Function works.\n");
		if (!works.empty())	
		{
			printf("Shutdown Function wakes thread.\n");
			pthread_cond_signal(&cond);
		}

		if (works.empty())
		{
			shutdown = true;
			//pthread_cond_destroy(&cond);
			return 0;
		}
		else
			return -1;
	}

	bool WorkQueue::IsShutdown() {
		return shutdown;
	}

	/////////////////////////////////////////////////Worker

	Worker::Worker(WorkQueue& queue) : workQueue(queue) {}

	Worker::~Worker() {
		// TODO: �ͷ���Դ
	}

	int Worker::Run() {
		// TODO: ����ѭ��
		printf("Thread %lld do this.\n", (long long)pthread_self());
		printf("workQueue.IsShutdown() is %d\n", workQueue.IsShutdown());
		while (!workQueue.IsShutdown())
		{
			Work* oneWork = workQueue.GetWork();
			if (!oneWork->NeedDelete())
			{
				if (oneWork != NULL)
					if(oneWork->DoWork() != 0)
						return -1;
			}
		}
		pthread_exit(NULL);
		return 0;
	}

	/////////////////////////////////////////////////WorkerThreadPool

	WorkerThreadPool::WorkerThreadPool() : worker(workQueue) {}

	WorkerThreadPool::~WorkerThreadPool() {
		// TODO: ��ɴ���
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

