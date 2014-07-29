#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include "processpool.h"

namespace mmtraining {

/////////////////////////////////////////////////Process

Process::Process() : target(0), pid(-1) {}
    
Process::Process(Runnable& t) : target(&t), pid(-1) {}

Process::~Process() {
 
}

int Process::Start() {
	pid = fork(); 
	if (pid == -1)
	{
		printf("Process fork failed.\n");
		return -1;
	}
	else if (pid == 0) 
	{
		Run();
		return 0;	
	}
	else 
		return 0;

}

int Process::GetId() const {
	if (pid > 0)
	{
		return pid;
	}
	else if(pid == 0) 
		return pid;
	else
	{
		printf("pid is wrong.");	
		return -1;
	}
	
}

int Process::Run() {
	if (pid == 0)
	{
		if (target != NULL)
		{
			return target->Run();	
		}
		else
		{
			return DoRun();	
		}
		exit(0);
	}
	else if (pid > 0)
	{
		return 0;
	}
	else
		return -1;
	return 0;
	
}

int Process::DoRun()
{
	if (pid == 0)
		exit(0);
	else
		return 0;
}

int Process::Kill() {
	if (pid == 0)
		exit(0);
	else if (pid > 0)
		return 0;		
	else
	{
		printf("pid is wrong.");	
		return -1;
	}
}

int Process::Wait() {
	if (pid > 0)
	{
		pid_t waitRet;
		waitRet = waitpid(pid, &processStatus, 0);
		if (waitRet == 0 || waitRet == -1) 
			return -1;
		else
			return 0;
	}
	else if (pid == 0)
	{
		return 0;	
	}
	else
	{
		printf("pid is wrong.\n");	
		return -1;
	}
}

/////////////////////////////////////////////////ProcessPool

ProcessPool::ProcessPool() {}
    
ProcessPool::~ProcessPool() {
 
}

int ProcessPool::Start(int procCount, Runnable& target) {
	for (int i=0; i<procCount; i++)	 
	{
		Process *newOneProcess = new Process(target);
		processes.push_back(newOneProcess);		
		if (newOneProcess->Start() != 0)
			return -1;
	}
    return 0;
}

int ProcessPool::KillAll() {
	for (Process* oneProcess : processes)		 
	{
		if (oneProcess->Kill() != 0)	
			return -1;
	}
    return 0;
}
    
int ProcessPool::WaitAll() {
	for (Process* oneProcess : processes)		 
	{
		if (oneProcess->Wait() == -1)	
			return -1;
	}
	return 0;
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
