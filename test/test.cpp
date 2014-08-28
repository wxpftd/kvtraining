#include <sys/types.h>
#include <iostream>  
#include "charqueue.h"
#include <cstring>
#include <sys/mman.h>
#include <sys/wait.h>

using namespace std;  

int main()  
{  
	char buffer[] = "Hello World.";
	CharQueue *charqueue = (CharQueue*)mmap(NULL, sizeof(CharQueue), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);

	charqueue->init();
	
	pid_t pid;
	if ((pid = fork()) == -1)
	{
		printf("fork failed.\n");	
		exit(-1);
	}
	
	
	if (pid > 0)	
	{
		for (int i=0; i<10000; i++)
		//while(1)
		{
			charqueue->push(buffer);
			fflush(stdout);
		}
		waitpid(pid, 0, 0);
		charqueue->destroy();
		munmap(charqueue, sizeof(CharQueue));
	}
	else if (pid == 0)
	{
		for (int i=0; i<10000; i++)
		//while(1)
		{
			charqueue->pop(buffer);
			fflush(stdout);
		}
		exit(0);
	}

	return 0;
}       
