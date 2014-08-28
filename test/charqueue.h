#ifndef __CHARQUEUE_H__
#define __CHARQUEUE_H__

#ifndef MAX_CHARQUEUE
#define MAX_CHARQUEUE 1000
#endif

#ifndef MAX_CharSeries 
#define MAX_CharSeries 1000 
#endif

#include <sys/shm.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <error.h>
#include <errno.h>
#include <fcntl.h>

class CharSeries
{
public:
	CharSeries();
	~CharSeries();
	CharSeries(char*);
	int getSeries(char*);
	int putSeries(char*);
	
private:
	char seriesSpace[MAX_CharSeries]; 
};


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
	//CharSeries queueSpace[MAX_CHARQUEUE];
	int space_shmkey;
	
	int head;
	int tail;
	int size;
	int tag;

	sem_t *sem_full;	
	sem_t *sem_empty;	
	//sem_t *sem_mutex_w;
	//sem_t *sem_mutex_r;
	sem_t *sem_mutex;
};

#endif
