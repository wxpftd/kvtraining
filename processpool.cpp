
#include "processpool.h"

namespace mmtraining {

/////////////////////////////////////////////////Process

Process::Process() : target(0) {}
    
Process::Process(Runnable& t) : target(&t) {}

Process::~Process() {
    // TODO: ÊÍ·Å×ÊÔŽ
}

int Process::Start() {
    // TODO: Íê³ÉŽúÂë
    return -1;
}

int Process::GetId() const {
    // TODO: Íê³ÉŽúÂë
    return -1;
}

int Process::Run() {
    if (target != NULL) {
        return target->Run();
    } else {
        return DoRun();
    }
}

int Process::Kill() {
    // TODO: Íê³ÉŽúÂë
    return -1;
}

int Process::Wait() {
    // TODO: Íê³ÉŽúÂë
    return -1;
}

/////////////////////////////////////////////////ProcessPool

ProcessPool::ProcessPool() {}
    
ProcessPool::~ProcessPool() {
    // TODO: ÊÍ·Å×ÊÔŽ
}

int ProcessPool::Start(int procCount, Runnable& target) {
    // TODO: Íê³ÉŽúÂë
    return -1;
}

int ProcessPool::KillAll() {
    // TODO: Íê³ÉŽúÂë
    return -1;
}
    
int ProcessPool::WaitAll() {
    // TODO: Íê³ÉŽúÂë
    return -1;
}

/////////////////////////////////////////////////TaskQueue

TaskQueue::TaskQueue() {
}
    
TaskQueue::~TaskQueue() {
}
    
int TaskQueue::AddTask(Task& task) {
    // TODO: Íê³ÉŽúÂë
    return -1;
}
    
int TaskQueue::GetTask(Task& task) {
    // TODO: Íê³ÉŽúÂë
    return -1;
}

/////////////////////////////////////////////////Processor

Processor::Processor(TaskQueue& queue, Task& tt) : taskQueue(queue), taskType(tt) {}

Processor::~Processor() {
    // TODO: ÊÍ·Å×ÊÔŽ
}

int Processor::Run() {
    // TODO: Íê³ÉŽúÂë
    return -1;
}

/////////////////////////////////////////////////TaskProcessPool

TaskProcessPool::TaskProcessPool(Task& dummy) : processor(taskQueue, dummy) {}

TaskProcessPool::~TaskProcessPool() {
    // TODO: ÊÍ·Å×ÊÔŽ
}

int TaskProcessPool::Start(int processCount) {
    return pool.Start(processCount, processor);
}

int TaskProcessPool::AddTask(Task& task) {
    return taskQueue.AddTask(task);
}

int TaskProcessPool::KillAll() {
    return pool.KillAll();
}

int TaskProcessPool::WaitAll() {
    return pool.WaitAll();
}

} // namespace mmtraining
