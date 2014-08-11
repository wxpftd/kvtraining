#include <sstream>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include "processpool.h"
#include <semaphore.h>

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
		delete oneProcess;
	}
	return 0;
}

/////////////////////////////////////////////////TaskQueue

TaskQueue::TaskQueue() {
	if (0 != sem_init(&sem, 0, 0))
	{
		printf("sem_init failed.\n");	
		exit(-1);
	}
	p_mutex = (pthread_mutex_t*)mmap(NULL, sizeof(pthread_mutex_t), PROT_WRITE|PROT_READ, MAP_SHARED|MAP_ANONYMOUS, -1, 0);	
	if (MAP_FAILED == p_mutex)
	{
		printf("mmap init failed.\n");	
		exit(-1);
	}
	if (0 != pthread_mutexattr_init(&attr))
	{
		printf("pthread_mutexattr_init() failed.\n");	
		exit(-1);
	}
	if (0 != pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED))
	{
		printf("pthread_mutexattr_setpshared() failed.\n");	
		exit(-1);
	}
	if (0 != pthread_mutex_init(p_mutex, &attr)) 
	{
		printf("pthread_mutex_init() failed.\n");	
		exit(-1);
	}
}
    
TaskQueue::~TaskQueue() {
	pthread_mutexattr_destroy(&attr);
	pthread_mutex_destroy(p_mutex);
}
    
int TaskQueue::AddTask(Task& task) {
	int ret = 0;
	std::string buffer;
	pthread_mutex_lock(p_mutex);
	tasks.push_back(&task);				
	ret = task.ToBuffer(buffer);
	if (0 != ret)
	{
		printf("ToBuffer() failed.\n");	
		return -1;
	}
	buffers.push_back(buffer);
	pthread_mutex_unlock(p_mutex);
	sem_post(&sem);
	printf("AddTask().\n");
	if (0 == ret)
		return 0;
	else
		return -1;
}
    
int TaskQueue::GetTask(Task& task) {
	std::string buffer;
	int ret = 0;
	while(tasks.empty()) 
	{
		printf("GetTask().\n");
		sem_wait(&sem);
	}
	pthread_mutex_lock(p_mutex);
	if (!tasks.empty())
	{
		task = *(tasks.front());
		tasks.pop_front();
		buffer = buffers.front();
		buffers.pop_front();
		task.FromBuffer(buffer);
	}
	pthread_mutex_unlock(p_mutex);
	if (0 == ret)
		return 0;
	else
		return -1;
}

int TaskQueue::ToBuffer(std::string &buffer)
{
	std::stringstream ss(buffer);	
	boost::archive::text_oarchive oa(ss);
	oa << (*this);
	return 0;
	return -1;	
}

int TaskQueue::FromBuffer(std::string &buffer)
{
	std::stringstream ss(buffer);
	boost::archive::text_iarchive ia(ss);
	ia >> (*this);
	return 0;
	return -1;		
}
/////////////////////////////////////////////////Processor

Processor::Processor(TaskQueue& queue, Task& tt) : taskQueue(queue), taskType(tt) {}

Processor::~Processor() {
	 
}

int Processor::Run() {
	while (1)
	{
		//printf("Processor::Run.\n");
		if (0 == taskQueue.GetTask(taskType))			
		{
			if (taskType.DoTask() != 0)
			{
				return -1;	
			}

		}
		else
		{ 
			return -1;
		}
	}
    return 0;
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
