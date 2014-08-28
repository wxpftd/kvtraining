#include <fcntl.h>
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
	//destroy();
}

int CharQueue::init()
{
	sem_close(sem_mutex);
	sem_close(sem_empty);
	sem_close(sem_full);
	sem_unlink("/sem_full");
	sem_unlink("/sem_empty");
	sem_unlink("/mutex");

	head = 0;
	tail = 0;
	size = MAX_CHARQUEUE;
	tag = 0;

	//sem_mutex_w = sem_open("/mutex_w", O_CREAT, 0644, 1); 
	//if (sem_mutex_w == SEM_FAILED || errno == EEXIST)
	//{
		//std::cerr << "sem_mutex_w fail : " << strerror(errno) << std::endl;
		//exit(-1);
	//}
//
	//sem_mutex_r = sem_open("/mutex_r", O_CREAT, 0644, 1); 
	//if (sem_mutex_r == SEM_FAILED || errno == EEXIST)
	//{
		//std::cerr << "sem_mutex_r fail : " << strerror(errno) << std::endl;
		//exit(-1);
	//}
	
	sem_mutex = sem_open("/mutex", O_CREAT, 0644, 1); 
	if (sem_mutex == SEM_FAILED || errno == EEXIST)
	{
		std::cerr << "sem_mutex fail : " << strerror(errno) << std::endl;
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
	space_shmkey = shmget(IPC_PRIVATE, sizeof(CharSeries)*MAX_CHARQUEUE, IPC_CREAT|SHM_EXEC);
	if (space_shmkey == -1)
	{
		std::cerr << "space_shmkey init failed:" << strerror(errno)  << std::endl;
		exit(-1);	
	}

	//key_t key;
	//int fd;
	//fd = open("/tmp/shm", O_RDWR|O_CREAT, 0644);
	//if (fd == -1)
	//{	
		//std::cerr << "open failed:"  << strerror(errno) << std::endl;
		//exit(-1);
	//}
	//key = ftok("/tmp/shm", 0);
	//if (key == -1)
	//{
		//std::cerr << "ftok failed:"  << strerror(errno) << std::endl;
		//exit(-1);
	//}
	//space_shmkey = shmget(key, sizeof(charseries)*max_charqueue, ipc_creat);
	//if (space_shmkey == -1)
	//{
		//std::cerr << "space_shmkey init failed:" << strerror(errno)  << std::endl;
		//exit(-1);	
	//}

	return 0;

}

int CharQueue::destroy()
{
	shmctl(space_shmkey, IPC_RMID, 0);
	sem_close(sem_mutex);
	sem_close(sem_empty);
	sem_close(sem_full);
	sem_unlink("/sem_full");
	sem_unlink("/sem_empty");
	sem_unlink("/mutex");

	return 0;

}

int CharQueue::push(char* buffer)
{
	//int value_mutex_w = 0;
	//std::cout << "sem_getvalue is " << sem_getvalue(sem_mutex_w, &value_mutex_w) << std::endl;
	//std::cout << "value_mutex_w is " << (value_mutex_w) << std::endl;
	while (tag!=0 && head==tail)
	{
		std::cout << "sem_wait(sem_empty)" << std::endl;	
		sem_wait(sem_empty);
	}
	sem_wait(sem_mutex);
	printf("push\n");
	printf("head is %d, tail is %d, tag is %d\n", head, tail, tag);
	CharSeries *oneSeries = (CharSeries*)shmat(space_shmkey, NULL, 0);
	oneSeries[tail].putSeries(buffer);
	shmdt(oneSeries);
	//queueSpace[tail].putSeries(buffer);
	tail = (tail + 1) % size;	
	if (tail == head)
		tag = 1;
	sem_post(sem_mutex);
	sem_post(sem_full);
	return 0;
	return -1;
}

int CharQueue::pop(char* buffer)
{
	//printf("pop\n");
	//int value_mutex_r = 0;
	//std::cout << "sem_getvalue is " << sem_getvalue(sem_mutex_r, &value_mutex_r) << std::endl;
	//std::cout << "value_mutex_r is " << (value_mutex_r) << std::endl;

	while (tag==0 && head==tail)
	{
		std::cout << "sem_wait(sem_full)" << std::endl;	
		sem_wait(sem_full);
	}
	sem_wait(sem_mutex);
	printf("pop\n");
	printf("head is %d, tail is %d, tag is %d\n", head, tail, tag);
	CharSeries *oneSeries = (CharSeries*)shmat(space_shmkey, NULL, 0);
	oneSeries[head].getSeries(buffer);
	shmdt(oneSeries);
	//queueSpace[head].getSeries(buffer);
	head = (1 + head) % size;
	tag = 0;
	sem_post(sem_mutex);
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

CharSeries::CharSeries()
{

}

CharSeries::CharSeries(char* buffer)
{
	putSeries(buffer);
}

CharSeries::~CharSeries()
{
}

int CharSeries::getSeries(char* buffer)
{
	//int seriesLenght = strlen(seriesSpace);
	//for (int pos = 0; pos <=seriesLenght ; pos++)
	//{
	//	buffer[0] = (*(seriesSpace + pos));		
	//}
	strcpy(seriesSpace, buffer);
	return 0;
	return -1;
}

int CharSeries::putSeries(char* buffer)
{
	//int bufferLenght = strlen(buffer);
	//for (int pos = 0; pos <=bufferLenght ; pos++)
	//{
	//	seriesSpace[0] = (*(buffer + pos));		
	//}
	strcpy(buffer, seriesSpace);
	return 0;
	return -1;
}


