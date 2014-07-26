#include "threadpool.h"
#include <iostream>
#include <pthread.h>

namespace mmtraining {

/////////////////////////////////////////////////Thread

Thread::Thread() : running(false), target(NULL) {tid = -1;}

Thread::Thread(Runnable& t) : running(false), target(&t) {tid = -1;}

Thread::~Thread() {
    // TODO: 释放资源
	target = NULL;
	running = false;
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
	std::cout << "Thread " << tid << " runs." << std::endl;
	pthread_exit(NULL);
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
	ptr->Run();
	return NULL;
}

/////////////////////////////////////////////////ThreadPool

ThreadPool::ThreadPool() {}

ThreadPool::~ThreadPool() {
    // TODO: 完成代码
}

int ThreadPool::Start(int threadCount, Runnable& target) {
    // TODO: 完成代码
    return -1;
}

int ThreadPool::JoinAll() {
    // TODO: 完成代码
    return -1;
}

///////////////////////////////////////////////WorkQueue

WorkQueue::WorkQueue() : shutdown(false) {
    // TODO: 初始化
}

WorkQueue::~WorkQueue() {
    // TODO: 释放资源
}

int WorkQueue::AddWork(Work* work) {
    // TODO: 完成代码
    return -1;
}

Work* WorkQueue::GetWork() {
    // TODO: 完成代码
    return NULL;
}

int WorkQueue::Shutdown() {
    // TODO: 完成代码
    return -1;
}

bool WorkQueue::IsShutdown() {
    return shutdown;
}

/////////////////////////////////////////////////Worker

Worker::Worker(WorkQueue& queue) : workQueue(queue) {}

Worker::~Worker() {
    // TODO: 释放资源
}

int Worker::Run() {
    // TODO: 工作循环
    return -1;
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

} // mmtraining

