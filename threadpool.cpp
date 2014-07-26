#include "threadpool.h"
#include <iostream>
#include <pthread.h>

namespace mmtraining {

/////////////////////////////////////////////////Thread

Thread::Thread() : running(false), target(NULL) {tid = -1;}

Thread::Thread(Runnable& t) : running(false), target(&t) {tid = -1;}

Thread::~Thread() {
    // TODO: �ͷ���Դ
	target = NULL;
	running = false;
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
	std::cout << "Thread " << tid << " runs." << std::endl;
	pthread_exit(NULL);
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
	ptr->Run();
	return NULL;
}

/////////////////////////////////////////////////ThreadPool

ThreadPool::ThreadPool() {}

ThreadPool::~ThreadPool() {
    // TODO: ��ɴ���
}

int ThreadPool::Start(int threadCount, Runnable& target) {
    // TODO: ��ɴ���
    return -1;
}

int ThreadPool::JoinAll() {
    // TODO: ��ɴ���
    return -1;
}

///////////////////////////////////////////////WorkQueue

WorkQueue::WorkQueue() : shutdown(false) {
    // TODO: ��ʼ��
}

WorkQueue::~WorkQueue() {
    // TODO: �ͷ���Դ
}

int WorkQueue::AddWork(Work* work) {
    // TODO: ��ɴ���
    return -1;
}

Work* WorkQueue::GetWork() {
    // TODO: ��ɴ���
    return NULL;
}

int WorkQueue::Shutdown() {
    // TODO: ��ɴ���
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
    return -1;
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

} // mmtraining

