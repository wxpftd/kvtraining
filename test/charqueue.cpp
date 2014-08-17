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
	mutex_shmkey_push = shmget(IPC_PRIVATE, sizeof(pthread_mutex_t), IPC_CREAT);	
	if (-1 == mutex_shmkey_push)
	{
		printf("mutex_shmkey_push's shmget failed.\n");	
		exit(-1);
	}
	q_mutex_push = (pthread_mutex_t*)shmat(mutex_shmkey_push, NULL, 0);
	if (0 != pthread_mutexattr_init(&q_attr_push))
	{
		printf("pthread_mutexattr_init() failed.\n");	
		exit(-1);
	}
	if (0 != pthread_mutexattr_setpshared(&q_attr_push, PTHREAD_PROCESS_SHARED))
	{
		printf("pthread_mutexattr_setpshared() failed.\n");	
		exit(-1);
	}
	if (0 != pthread_mutex_init(q_mutex_push, &q_attr_push)) 
	{
		printf("pthread_mutex_init() failed.\n");	
		exit(-1);
	}

	mutex_shmkey_pop= shmget(IPC_PRIVATE, sizeof(pthread_mutex_t), IPC_CREAT);	
	if (-1 == mutex_shmkey_pop)
	{
		printf("mutex_shmkey_pop's shmget failed.\n");	
		exit(-1);
	}
	q_mutex_pop = (pthread_mutex_t*)shmat(mutex_shmkey_pop, NULL, 0);
	if (0 != pthread_mutexattr_init(&q_attr_pop))
	{
		printf("pthread_mutexattr_init() failed.\n");	
		exit(-1);
	}
	if (0 != pthread_mutexattr_setpshared(&q_attr_pop, PTHREAD_PROCESS_SHARED))
	{
		printf("pthread_mutexattr_setpshared() failed.\n");	
		exit(-1);
	}
	if (0 != pthread_mutex_init(q_mutex_pop, &q_attr_pop)) 
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
	long long length = strlen(buffer);
	queueSpace = (char*)shmat(space_shmkey, NULL, 0);
	q_mutex_push = (pthread_mutex_t*)shmat(mutex_shmkey_push, NULL, 0);
	pthread_mutex_lock(q_mutex_push);
	//std::cout << "push" << std::endl;
	if (tail > head)
	{
	//	std::cout << "tail > head" << std::endl;
		if (head+size-tail < length+1) 	
		{
			tag = 1;
		}
	}
	else if (tail < head)
	{
	//	std::cout << "tail < head" << std::endl;
		if (head-tail < length+1)	
			tag = 1;
	}
	//std::cout << "tag is " << tag << std::endl;
	while (tag!=0)
	{
		printf("sem_wait(&q_sem_full)\n");	
		sem_wait(&q_sem_full);
	}
	strcpy(queueSpace+head, buffer);
	tail = (length+1+tail) % size;
	if (tail == head)
		tag = 1;
	//std::cout << "head is " << head << "tail is " << tail << std::endl;
	pthread_mutex_unlock(q_mutex_push);
	sem_post(&q_sem_empty);
	return 0;
	return -1;
}

int CharQueue::pop(char* buffer)
{
	std::cout << "pop" << std::endl;
	queueSpace = (char*)shmat(space_shmkey, NULL, 0);
	q_mutex_pop = (pthread_mutex_t*)shmat(mutex_shmkey_pop, NULL, 0);
	pthread_mutex_lock(q_mutex_pop);
	std::cout << "tag is " << tag << "head is " << head << "tail is " << tail << std::endl;
	while (tag == 0 && head == tail)
		sem_wait(&q_sem_empty);
	strcpy(buffer, queueSpace+head);
	head += (strlen(buffer) + 1);
	tag = 0;
	std::cout << "head is " << head << "tail is " << tail << std::endl;
	pthread_mutex_unlock(q_mutex_pop);
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

