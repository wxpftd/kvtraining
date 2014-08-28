
#ifndef MM_TRAINING_PROCESS_POOL_H
#define MM_TRAINING_PROCESS_POOL_H

#include <string>
#include <deque>
#include <semaphore.h>
#include <vector>
#include "runnable.h"
#include <pthread.h>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/deque.hpp>
#include "charqueue.h"

namespace mmtraining {

/**
 * ������
 */
class Process {
public:
    /**
     * ���캯��
     */
    Process();
    
    /**
     * ���캯��
     */
    Process(Runnable& t);

    /**
     * ��������
     */
    virtual ~Process();

    /**
     * ��������
     * @return 0: �ɹ�, -1: ʧ��
     */
    int Start();
    
    /**
     * ��ȡ����id
     */
    int GetId() const;

    /**
     * ���н��̴����߼�
     * @return 0: �ɹ�, -1: ʧ��
     */
    int Run();
    
    /**
     * ��ֹ����
     */
    int Kill();

    /**
     * �ȴ������˳��������ս�����Դ
     * @return 0 �ɹ�, -1 ʧ��
     */
    int Wait();

protected:

    /**
     * ���̴����߼�
     */
    virtual int DoRun();
    
    Runnable* target;

private:
	int processStatus;
	pid_t pid;
};

/**
 * ���̳���
 */
class ProcessPool {
public:
    /**
     * ���캯��
     */
    ProcessPool();
    
    /**
     * ��������
     */
    ~ProcessPool();

    /**
     * �������̳�
     */
    int Start(int procCount, Runnable& target);
    
    /**
     * ��ֹ���н���
     */
    int KillAll();
    
    /**
     * �ȴ����н����˳��������ս�����Դ
     * @return 0 �ɹ�, -1 ʧ��
     */
    int WaitAll();
private:
    typedef std::vector<Process*> ProcessVec;

    ProcessVec processes;


};

/**
 * �������
 */
class Task {
public:
    /**
     * �����������ͷ���Դ
     */
    virtual ~Task() {}

    /**
     * �������߼�
     * @return 0 ����ɹ�, -1 ����ʧ��
     */
    virtual int DoTask() = 0;
    
    /**
     * ���л�
     */
    virtual int ToBuffer(std::string& buffer) = 0;
    
    /**
     * �����л�
     */
    virtual int FromBuffer(std::string& buffer) = 0;
};

/**
 * ���̰�ȫ�Ĺ����ڴ滷���������
 */
class TaskQueue {
public:
    /**
     * ���캯��
     */
    TaskQueue();
    
    /**
     * ��������
     */
    ~TaskQueue();

	/**
	 * ������г�ʼ��
	 */
	int init();

    /**
	 * �����������
	 */
	int destroy();

    /**
     * �������, ������һ�����̴���
     * @return 0: �ɹ�, -1: ʧ��
     */
    int AddTask(Task& task);
    
    /**
     * ��ȡ����, ���п�ʱ�ȴ�
     * @return 0: �ɹ�, -1: ʧ��
     */
    int GetTask(Task& task);

private:
	CharQueue *tasks;
};

/**
 * ��������
 */
class Processor : public Runnable {
public:
    /**
     * ���캯��
     * @param queue �������
     * @param tt ��������
     */
    Processor(TaskQueue& queue, Task& tt);

    /**
     * ��������
     */
    ~Processor();

	/**
	 * ��ʼ��
	 */
	int init();

	/**
	 * ���ٶ���
	 */
	int destroy();

	/**
     * �����߼�, �����������ȡ������������
     */
    int Run();
    
private:
    TaskQueue& taskQueue;
    Task& taskType;
};

/**
 * ������̳�
 */
class TaskProcessPool {
public:
    /**
     * ���캯��
     */
    TaskProcessPool(Task& taskType);

    /**
     * ��������
     */
    ~TaskProcessPool();

	/**
	 * ��ʼ��
	 */
	int init();

	/**
	 * ���ٶ���
	 */
	int destroy();

    /**
     * ��������
     * @param threadCount �����Ľ�����
     * @return 0 �ɹ�, -1 ʧ��
     */
    int Start(int processCount);

    /**
     * ������񣬻���������̴���
     * @param task ����
     * return 0 �ɹ�, -1 ʧ��
     */
    int AddTask(Task& task);

    /**
     * ��ֹ���н���
     */
    int KillAll();

    /**
     * �ȴ����н����˳��������ս�����Դ
     * @return 0 �ɹ�, -1 ʧ��
     */
    int WaitAll();

private:

    TaskQueue taskQueue;
    Processor processor;
    ProcessPool pool;
};

} // namespace mmtraining

#endif
