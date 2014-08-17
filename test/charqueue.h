#ifndef __charqueue__
#define __charqueue__
#define MAX_CHARQUEUE 10000

#include <sys/shm.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <error.h>
#include <errno.h>
#include <fcntl.h>

class CharQueue 
{
public:
	CharQueue();
	~CharQueue();
	int init();
	int destroy();
	int push(char*); 
	int pop(char*);
	int clear(); 

private:
	key_t space_shmkey;
	char* queueSpace;
	int head;
	int tail;
	int size;
	int tag;

	sem_t *sem_full;	
	sem_t *sem_empty;	
	sem_t *sem_mutex_w;
	sem_t *sem_mutex_r;
};

#endif
