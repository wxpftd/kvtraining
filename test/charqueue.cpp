#include <iostream>
#include <cstdio>
#include <cstring>
#include "charqueue.h"
#include <sys/shm.h>


CharQueue::CharQueue():head(0), tail(0), size(MAX_CHARQUEUE), tag(0)
{
	if (0 != sem_init(&q_sem_full, 0, 0))
	{
		printf("sem_init failed.\n");	
		exit(-1);
	}
	if (0 != sem_init(&q_sem_empty, 0, 0))
	{
		printf("sem_init failed.\n");	
		exit(-1);
	}
	mutex_shmkey = shmget(IPC_PRIVATE, sizeof(pthread_mutex_t), IPC_CREAT);	
	if (-1 == mutex_shmkey)
	{
		printf("mutex_shmkey's shmget failed.\n");	
		exit(-1);
	}
	q_mutex = (pthread_mutex_t*)shmat(mutex_shmkey, NULL, 0);
	if (0 != pthread_mutexattr_init(&q_attr))
	{
		printf("pthread_mutexattr_init() failed.\n");	
		exit(-1);
	}
	if (0 != pthread_mutexattr_setpshared(&q_attr, PTHREAD_PROCESS_SHARED))
	{
		printf("pthread_mutexattr_setpshared() failed.\n");	
		exit(-1);
	}
	if (0 != pthread_mutex_init(q_mutex, &q_attr)) 
	{
		printf("pthread_mutex_init() failed.\n");	
		exit(-1);
	}
	if ((space_shmkey = shmget(IPC_PRIVATE, sizeof(char)*MAX_CHARQUEUE, IPC_CREAT)) == -1)
	{
		std::cerr << "space_shmkey's shmget failed." << std::endl;
	}
}

CharQueue::~CharQueue()
{
	shmctl(IPC_PRIVATE, IPC_RMID, 0);
}

int CharQueue::push(char* buffer)
{
	pthread_mutex_lock(q_mutex);
	long long length = strlen(buffer);
	queueSpace = (char*)shmat(space_shmkey, NULL, 0);
	q_mutex = (pthread_mutex_t*)shmat(mutex_shmkey, NULL, 0);
	if (tail > head)
	{
		if (head+size-tail < length+1) 	
			tag = 1;
	}
	else if (tail < head)
	{
		if (head-tail < length+1)	
			tag = 1;
	}
	else
	{
		tag = 1;	
	}
	while (tag!=0)
		sem_wait(&q_sem_full);
	strcpy(queueSpace+head, buffer);
	tail += (length+1+tail) % size;
	pthread_mutex_unlock(q_mutex);
	sem_post(&q_sem_empty);
	return 0;
	return -1;
}

int CharQueue::pop(char* buffer)
{
	pthread_mutex_lock(q_mutex);
	queueSpace = (char*)shmat(space_shmkey, NULL, 0);
	q_mutex = (pthread_mutex_t*)shmat(mutex_shmkey, NULL, 0);
	while (tag == 0 && head == tail)
		sem_wait(&q_sem_empty);
	strcpy(buffer, queueSpace+head);
	head += (strlen(buffer) + 1);
	tag = 0;
	pthread_mutex_unlock(q_mutex);
	sem_post(&q_sem_full);
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

