
#include "threadpool.h"

namespace mmtraining {

/////////////////////////////////////////////////Thread

Thread::Thread() : running(false), target(NULL) {}

Thread::Thread(Runnable& t) : running(false), target(&t) {}

Thread::~Thread() {
    // TODO: �ͷ���Դ
}

int Thread::Start() {
    // TODO: �����߳�, ���� Run
    return -1;
}

pthread_t Thread::GetId() const {
    // TODO: �����߳�, ���� Run
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
    return 0;
}

bool Thread::IsRunning() const {
    return running;
}

int Thread::Join() {
    // TODO: ��ɴ���
    return -1;
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

