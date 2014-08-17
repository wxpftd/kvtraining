#include <iostream>
#include <cstdio>
#include <cstring>
#include "charqueue.h"
#include <sys/shm.h>


CharQueue::CharQueue():head(0), tail(0), size(MAX_CHARQUEUE), tag(0)
{
	if (0 != sem_init(&sem_full, 1, 0))
	{
		printf("sem_full failed.\n");	
		exit(-1);
	}
	if (0 != sem_init(&sem_empty, 1, 0))
	{
		printf("sem_empty failed.\n");	
		exit(-1);
	}
	if (0 != sem_init(&sem_mutex_r, 1, 1))
	{
		printf("sem_mutex_r failed.\n");	
		exit(-1);
	}
	sem_mutex_w = sem_open("/tmp/pro_mutex", O_CREAT|O_EXCL, S_IRWXU, 1); 
	if (sem_mutex_w == SEM_FAILED && errno == EEXIST)
	{
		std::cout << "sem_open fail." << std::endl;	
		exit(-1);
	}

	if ((space_shmkey = shmget(IPC_PRIVATE, sizeof(char)*MAX_CHARQUEUE, IPC_CREAT)) == -1)
	{
		std::cerr << "space_shmkey's shmget failed." << std::endl;
	}
}

CharQueue::~CharQueue()
{
	//shmctl(IPC_PRIVATE, IPC_RMID, 0);
	sem_close(sem_mutex_w);
	sem_destroy(&sem_mutex_r);
	sem_destroy(&sem_empty);
	sem_destroy(&sem_full);
}

int CharQueue::push(char* buffer)
{
	int *value_mutex_w;
	sem_getvalue(sem_mutex_w, value_mutex_w);
	std::cout << (*value_mutex_w) << std::endl;
	long long length = strlen(buffer);
	queueSpace = (char*)shmat(space_shmkey, NULL, 0);
	sem_wait(sem_mutex_w);
	std::cout << "push" << std::endl;
	if (tail > head)
	{
		std::cout << "tail > head" << std::endl;
		if (head+size-tail < length+1) 	
		{
			tag = 1;
		}
	}
	else if (tail < head)
	{
		std::cout << "tail < head" << std::endl;
		if (head-tail < length+1)	
			tag = 1;
	}
	std::cout << "tag is " << tag << std::endl;
	while (tag!=0)
	{
		printf("sem_wait(&sem_full)\n");	
		sem_wait(&sem_empty);
	}
	//strcpy(queueSpace+head, buffer);
	tail = (length+1+tail) % size;
	if (tail == head)
		tag = 1;
	std::cout << "head is " << head << "tail is " << tail << std::endl;
	sem_post(sem_mutex_w);
	sem_post(&sem_full);
	return 0;
	return -1;
}

int CharQueue::pop(char* buffer)
{
	queueSpace = (char*)shmat(space_shmkey, NULL, 0);
	sem_wait(&sem_mutex_r);
	std::cout << "pop" << std::endl;
	std::cout << "tag is " << tag << "head is " << head << "tail is " << tail << std::endl;
	while (tag == 0 && head == tail)
		sem_wait(&sem_full);
	//strcpy(buffer, queueSpace+head);
	head += (strlen(buffer) + 1);
	tag = 0;
	std::cout << "head is " << head << "tail is " << tail << std::endl;
	sem_post(&sem_mutex_r);
	sem_post(&sem_empty);
	return 0;
	return -1;
}

int CharQueue::clear()
{
	head = tail = 0;
	tag = 0;
	return 0;
	return -1;
}

