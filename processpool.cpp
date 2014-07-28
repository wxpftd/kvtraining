
#include "processpool.h"

namespace mmtraining {

/////////////////////////////////////////////////Process

Process::Process() : target(0) {}
    
Process::Process(Runnable& t) : target(&t) {}

Process::~Process() {
 
}

int Process::Start() {
 
    return -1;
}

int Process::GetId() const {
 
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
 
    return -1;
}

int Process::Wait() {
 
    return -1;
}

/////////////////////////////////////////////////ProcessPool

ProcessPool::ProcessPool() {}
    
ProcessPool::~ProcessPool() {
 
}

int ProcessPool::Start(int procCount, Runnable& target) {
 
    return -1;
}

int ProcessPool::KillAll() {
 
    return -1;
}
    
int ProcessPool::WaitAll() {
 
    return -1;
}

/////////////////////////////////////////////////TaskQueue

TaskQueue::TaskQueue() {
}
    
TaskQueue::~TaskQueue() {
}
    
int TaskQueue::AddTask(Task& task) {
 
    return -1;
}
    
int TaskQueue::GetTask(Task& task) {
 
    return -1;
}

/////////////////////////////////////////////////Processor

Processor::Processor(TaskQueue& queue, Task& tt) : taskQueue(queue), taskType(tt) {}

Processor::~Processor() {
 
}

int Processor::Run() {
 
    return -1;
}

/////////////////////////////////////////////////TaskProcessPool

TaskProcessPool::TaskProcessPool(Task& dummy) : processor(taskQueue, dummy) {}

TaskProcessPool::~TaskProcessPool() {
 
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
