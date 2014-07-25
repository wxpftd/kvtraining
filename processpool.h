
#ifndef MM_TRAINING_PROCESS_POOL_H
#define MM_TRAINING_PROCESS_POOL_H

#include <string>
#include "runnable.h"

namespace mmtraining {

/**
 * 进程类
 */
class Process {
public:
    /**
     * 构造函数
     */
    Process();
    
    /**
     * 构造函数
     */
    Process(Runnable& t);

    /**
     * 析构函数
     */
    virtual ~Process();

    /**
     * 启动进程
     * @return 0: 成功, -1: 失败
     */
    int Start();
    
    /**
     * 获取进程id
     */
    int GetId() const;

    /**
     * 运行进程处理逻辑
     * @return 0: 成功, -1: 失败
     */
    int Run();
    
    /**
     * 终止进程
     */
    int Kill();

    /**
     * 等待进程退出，并回收进程资源
     * @return 0 成功, -1 失败
     */
    int Wait();

protected:

    /**
     * 进程处理逻辑
     */
    virtual int DoRun() = 0;
    
    Runnable* target;
};

/**
 * 进程池类
 */
class ProcessPool {
public:
    /**
     * 构造函数
     */
    ProcessPool();
    
    /**
     * 析构函数
     */
    ~ProcessPool();

    /**
     * 启动进程池
     */
    int Start(int procCount, Runnable& target);
    
    /**
     * 终止所有进程
     */
    int KillAll();
    
    /**
     * 等待所有进程退出，并回收进程资源
     * @return 0 成功, -1 失败
     */
    int WaitAll();
};

/**
 * 任务基类
 */
class Task {
public:
    /**
     * 析构函数，释放资源
     */
    virtual ~Task() {}

    /**
     * 任务处理逻辑
     * @return 0 处理成功, -1 处理失败
     */
    virtual int DoTask() = 0;
    
    /**
     * 序列化
     */
    virtual int ToBuffer(std::string& buffer) = 0;
    
    /**
     * 反序列化
     */
    virtual int FromBuffer(std::string& buffer) = 0;
};

/**
 * 进程安全的共享内存环形任务队列
 */
class TaskQueue {
public:
    /**
     * 构造函数
     */
    TaskQueue();
    
    /**
     * 析构函数
     */
    ~TaskQueue();
    
    /**
     * 添加任务, 并唤醒一个进程处理
     * @return 0: 成功, -1: 失败
     */
    int AddTask(Task& task);
    
    /**
     * 获取任务, 队列空时等待
     * @return 0: 成功, -1: 失败
     */
    int GetTask(Task& task);
};

/**
 * 任务处理器
 */
class Processor : public Runnable {
public:
    /**
     * 构造函数
     * @param queue 任务队列
     * @param tt 任务类型
     */
    Processor(TaskQueue& queue, Task& tt);

    /**
     * 析构函数
     */
    ~Processor();

	/**
     * 处理逻辑, 从任务队列中取出工作并处理
     */
    int Run();
    
private:
    
    TaskQueue& taskQueue;
    Task& taskType;
};

/**
 * 任务进程池
 */
class TaskProcessPool {
public:
    /**
     * 构造函数
     */
    TaskProcessPool(Task& taskType);

    /**
     * 析构函数
     */
    ~TaskProcessPool();

    /**
     * 启动进程
     * @param threadCount 启动的进程数
     * @return 0 成功, -1 失败
     */
    int Start(int processCount);

    /**
     * 添加任务，唤醒任务进程处理
     * @param task 任务
     * return 0 成功, -1 失败
     */
    int AddTask(Task& task);

    /**
     * 终止所有进程
     */
    int KillAll();

    /**
     * 等待所有进程退出，并回收进程资源
     * @return 0 成功, -1 失败
     */
    int WaitAll();

private:

    TaskQueue taskQueue;
    Processor processor;
    ProcessPool pool;
};

} // namespace mmtraining

#endif
