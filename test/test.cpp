#include <sys/types.h>
#include <iostream>  
#include "charqueue.h"
#include <cstring>
#include <sys/mman.h>

using namespace std;  

int main()  
{  
	char buffer[] = "Hello World.";
	CharQueue *charqueue;
	charqueue = (CharQueue*)mmap(NULL, sizeof(CharQueue), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
	charqueue->init();
	
	pid_t pid;
	if ((pid = fork()) == -1)
	{
		printf("fork failed.\n");	
		exit(-1);
	}
	
	
	if (pid > 0)	
	{
		//for (int i=0; i<1000000; i++)
		while(1)
		{
			//cout << "pid is " << pid << endl;
			charqueue->push(buffer);
			fflush(stdout);
		}
		waitpid(pid, 0, 0);
		//charqueue->destroy();
	}
	else if (pid == 0)
	{
		//for (int i=0; i<100; i++)
		while(1)
		{
			charqueue->pop(buffer);
			//cout << buffer << endl;
			fflush(stdout);
		}
		exit(0);
	}

	return 0;
}       
