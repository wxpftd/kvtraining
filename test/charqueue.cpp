#include <iostream>
#include <cstdio>
#include <cstring>
#include "charqueue.h"
#include <sys/shm.h>


CharQueue::CharQueue():head(0), tail(0), size(MAX_CHARQUEUE), tag(0)
{
	init();	
}

CharQueue::~CharQueue()
{
	destroy();
}

int CharQueue::init()
{
	sem_close(sem_mutex_w);
	sem_close(sem_mutex_r);
	sem_close(sem_empty);
	sem_close(sem_full);
	sem_unlink("/sem_full");
	sem_unlink("/sem_empty");
	sem_unlink("/mutex_w");
	sem_unlink("/mutex_r");

	head = 0;
	tail = 0;
	size = MAX_CHARQUEUE;
	tag = 0;

	sem_mutex_w = sem_open("/mutex_w", O_CREAT, 0644, 1); 
	if (sem_mutex_w == SEM_FAILED || errno == EEXIST)
	{
		std::cerr << "sem_mutex_w fail : " << strerror(errno) << std::endl;
		exit(-1);
	}

	sem_mutex_r = sem_open("/mutex_r", O_CREAT, 0644, 1); 
	if (sem_mutex_r == SEM_FAILED || errno == EEXIST)
	{
		std::cerr << "sem_mutex_r fail : " << strerror(errno) << std::endl;
		exit(-1);
	}

	sem_full = sem_open("/sem_full", O_CREAT, 0644, 0); 
	if (sem_full == SEM_FAILED || errno == EEXIST)
	{
		std::cerr << "sem_full fail : " << strerror(errno) << std::endl;
		exit(-1);
	}

	sem_empty = sem_open("/sem_empty", O_CREAT, 0644, 0); 
	if (sem_empty == SEM_FAILED || errno == EEXIST)
	{
		std::cerr << "sem_empty fail : " << strerror(errno) << std::endl;
		exit(-1);
	}
	if ((space_shmkey = shmget(IPC_PRIVATE, sizeof(char)*MAX_CHARQUEUE, IPC_CREAT)) == -1)
	{
		std::cerr << "space_shmkey's shmget failed." << std::endl;
	}
	//int value_mutex_w = 0;
	//sem_getvalue(sem_mutex_w, &value_mutex_w);
	//std::cout << "value_mutex_w is " << (value_mutex_w) << std::endl;

	return 0;

}

int CharQueue::destroy()
{
	shmctl(space_shmkey, IPC_RMID, 0);
	sem_close(sem_mutex_w);
	sem_close(sem_mutex_r);
	sem_close(sem_empty);
	sem_close(sem_full);
	sem_unlink("/sem_full");
	sem_unlink("/sem_empty");
	sem_unlink("/mutex_w");
	sem_unlink("/mutex_r");

	return 0;

}

int CharQueue::push(char* buffer)
{
	//int value_mutex_w = 0;
	//std::cout << "sem_getvalue is " << sem_getvalue(sem_mutex_w, &value_mutex_w) << std::endl;
	//std::cout << "value_mutex_w is " << (value_mutex_w) << std::endl;
	int length = strlen(buffer);
	queueSpace = (char*)shmat(space_shmkey, NULL, 0);
	sem_wait(sem_mutex_w);
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
		printf("sem_wait(&sem_full)\n");	
		sem_wait(sem_empty);
	}
	strcpy(queueSpace+head, buffer);
	tail = (length+1+tail) % size;
	if (tail == head)
		tag = 1;
	std::cout << "head is " << head << std::endl << "tail is " << tail << std::endl;
	sem_post(sem_mutex_w);
	sem_post(sem_full);
	return 0;
	return -1;
}

int CharQueue::pop(char* buffer)
{
	queueSpace = (char*)shmat(space_shmkey, NULL, 0);
	sem_wait(sem_mutex_r);
	//std::cout << "pop" << std::endl;
	//std::cout << "tag is " << tag << "head is " << head << "tail is " << tail << std::endl;
	while (tag == 0 && head == tail)
		sem_wait(sem_full);
	//strcpy(buffer, queueSpace+head);
	head = (strlen(buffer) + 1 + head) % size;
	tag = 0;
	//std::cout << "head is " << head << "tail is " << tail << std::endl;
	sem_post(sem_mutex_r);
	sem_post(sem_empty);
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

