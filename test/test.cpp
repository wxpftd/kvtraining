#include <sys/types.h>
#include <iostream>  
#include "charqueue.h"
#include <cstring>
#include <sys/mman.h>

using namespace std;  

int main()  
{  
	char buffer[] = "Hello World.";
	CharQueue *charqueue = new CharQueue();
	charqueue = (CharQueue*)mmap(NULL, sizeof(CharQueue), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);

	pid_t pid;
	if ((pid = fork()) == -1)
	{
		printf("fork failed.\n");	
		exit(-1);
	}
	
	
	//if (pid > 0)	
	//{
	//	while(1)
	//	{
	//		charqueue->push(buffer);
	//	}
	//}
	//else if (pid == 0)
	//{
	//	while (1)
	//	{
	//		cout << "pid is " << pid << endl;
	//		charqueue->pop(buffer);
	//		cout << buffer << endl;
	//	}
	//}

	return 0;
}       
