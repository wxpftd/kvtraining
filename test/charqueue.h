#ifndef __charqueue__
#define __charqueue__
#define MAX_CHARQUEUE 10000

#include <sys/shm.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <error.h>
#include <fcntl.h>

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
	char* queueSpace;
	long long head;
	long long tail;
	long long size;
	int tag;
	

	sem_t sem_full;	
	sem_t sem_empty;	
	sem_t *sem_mutex_w;
	sem_t sem_mutex_r;
};

#endif
