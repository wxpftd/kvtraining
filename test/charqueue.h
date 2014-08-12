#ifndef __charqueue__
#define __charqueue__
#define MAX_CHARQUEUE 1000000

#include <sys/shm.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/wait.h>

class CharQueue 
{
public:
	CharQueue();
	~CharQueue();
	int push(char*); 
	int pop(char*);
	int clear(); 

private:
	key_t space_shmkey;
	key_t mutex_shmkey;
	char* queueSpace;
	long long head;
	long long tail;
	long long size;
	int tag;
	

	pthread_mutexattr_t q_attr;
	pthread_mutex_t *q_mutex;
	sem_t q_sem_full;	
	sem_t q_sem_empty;	
};

#endif
