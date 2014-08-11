#include <sys/mman.h>
#include <iostream>  
#include <sstream>  
#include <string>  
#include <pthread.h>
#include <fstream>  
#include <fcntl.h>
#include <unistd.h>
using namespace std;  

void init_mutex(void)
{
	int ret;
	p_mutex = (pthread_mutex_t*)mmap(NULL, sizeof(pthread_mutex_t), PROT_WRITE|PROT_READ, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
	if (MAP_FAILED == p_mutex)
	{
		cerr << "mmap init failed." << endl;	
		exit(-1);
	}

	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	ret = pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
	if (0 != ret)
	{
		cerr << "pthread_mutexattr_setpshared failed." << endl;	
		exit(-1);
	}
	pthread_mutex_init(p_mutex, &attr);
}

class CharQueue 
{
public:
	CharQueue();
	~CharQueue();

	void push(char*);
	char* pop();

private:
	long long head;
	long long tail;

	pthread_mutex_t *p_mutex;

};


int main()  
{  
	return 0;
}       
