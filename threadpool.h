#ifndef MM_TRANING_THREAD_POOL_H
#define MM_TRANING_THREAD_POOL_H

#include <pthread.h>
#include <deque>
#include <vector>
#include "runnable.h"

namespace mmtraining {

/**
 * �߳�
 */
class Thread : public Runnable {
public:
    /**
     * ���캯��
     */
    Thread();
    
    /**
     * ���캯��
     */
    Thread(Runnable& t);

    /**
     * ��������
     */
    virtual ~Thread();
    
    /**
     * �����߳�
     * @return 0 �ɹ�, -1 ʧ��
     */
    int Start();
    
    /**
     * ��ȡ�߳�id
     */
    pthread_t GetId() const;

    /**
     * �̴߳���
     */
    int Run();
    
    /**
     * ��ȡ�߳��Ƿ���������
     */
    bool IsRunning() const;
    
    /**
     * �ȴ��߳��˳����������߳���Դ
     * @return 0 �ɹ�, -1 ʧ��
     */
    int Join();
    
protected:
    /**
     * �̴߳���
     */
    virtual int DoRun();
    
    bool running;
    Runnable* target;
};

/**
 * �̳߳�, ά����һ���߳�
 */
class ThreadPool {
public:
    /**
     * ���캯��
     */
    ThreadPool();

    /**
     * ��������
     */
    virtual ~ThreadPool();

    /**
     * �����߳�, ����Ŀ���߼�
     * @param threadCount �������߳���
     * @param target Ŀ�������߼�
     * @return 0 �ɹ�, -1 ʧ��
     */
    virtual int Start(int threadCount, Runnable& target);

    /**
     * �ȴ������߳��˳����������߳���Դ
     * @return 0 �ɹ�, -1 ʧ��
     */
    int JoinAll();

protected:
    typedef std::vector<Thread*> ThreadVec;

    ThreadVec threads;
};

/**
 * ��������
 */
class Work {
public:
    /**
     * �����������ͷ���Դ
     */
    virtual ~Work() {}
    
    /**
     * �Ƿ���Ҫ�� delete
     */
    virtual bool NeedDelete() const = 0;

    /**
     * ���������߼�
     * @return 0 ����ɹ�, -1 ����ʧ��
     */
    virtual int DoWork() = 0;
};

/**
 * �̰߳�ȫ��FIFO��������
 */
class WorkQueue {
public:
    /**
     * ���캯��, ��ʼ����������
     */
    WorkQueue();

    /**
     * ��������, �ͷ���Դ
     */
    ~WorkQueue();

    /**
     * �����������������, ������һ���̴߳���
     * @param work ��������
     * @return 0 �ɹ�, -1 ʧ��
     */
    int AddWork(Work* work);

    /**
     * �Ӷ�����ȡ��һ������,
     * ������Ϊ��, ��ȴ�������
     * @return ����
     */
    Work* GetWork();

    /**
     * �رն���, �������й����߳�
     * @return 0 �ɹ�, -1 ʧ��
     */
    int Shutdown();

    /**
     * �Ƿ񱻹ر�
     */
    bool IsShutdown();

private:
    typedef std::deque<Work*> Queue;

    pthread_mutex_t mutex;
    pthread_cond_t cond;
    bool shutdown;
    Queue works;
};

/**
 * ���������߼�, ���ϴӹ��������л�ȡ����������
 */
class Worker : public Runnable {
public:
    /**
     * ���캯��
     * @param queue ��������
     */
    Worker(WorkQueue& queue);

    /**
     * ��������
     */
    ~Worker();

	/**
     * �����߼�, �ӹ���������ȡ������������
     */
    int Run();
    
private:
    
    WorkQueue& workQueue;
};

/**
 * �����̳߳�, ά����һ���������к�һ�鹤���߳�,
 * ͨ�����������з������������������̹߳���
 */
class WorkerThreadPool {
public:
    /**
     * ���캯��
     */
    WorkerThreadPool();

    /**
     * ��������
     */
    ~WorkerThreadPool();

    /**
     * �����߳�
     * @param threadCount �������߳���
     * @return 0 �ɹ�, -1 ʧ��
     */
    int Start(int threadCount);

    /**
     * ��ӹ��������ѹ����̴߳���
     * @param work ����
     * return 0 �ɹ�, -1 ʧ��
     */
    int AddWork(Work* work);

    /**
     * ֹͣ�����߳�
     */
    int Shutdown();

    /**
     * �ȴ������߳��˳����������߳���Դ
     * @return 0 �ɹ�, -1 ʧ��
     */
    int JoinAll();

protected:

    /**
     * �����߳�
     * @param threadCount �������߳���
     * @return 0 �ɹ�, -1 ʧ��
     */
    int Start(int threadCount, Runnable& target);

    WorkQueue workQueue;
    Worker worker;
    ThreadPool pool;
};

} // mmtraining

#endif // MM_TRANNING_THREAD_POOL_H
