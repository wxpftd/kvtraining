
#ifndef MM_TRAINING_PROCESS_POOL_H
#define MM_TRAINING_PROCESS_POOL_H

#include <string>
#include "runnable.h"

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
    virtual int DoRun() = 0;
    
    Runnable* target;
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
     * �������, ������һ�����̴���
     * @return 0: �ɹ�, -1: ʧ��
     */
    int AddTask(Task& task);
    
    /**
     * ��ȡ����, ���п�ʱ�ȴ�
     * @return 0: �ɹ�, -1: ʧ��
     */
    int GetTask(Task& task);
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
