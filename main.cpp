#include <iostream>
#include <unistd.h>
#include <string>
#include <sys/socket.h>
#include "echoser.h"
#include "threadpool.h"

using namespace std;
using namespace mmtraining;

int main()
{
	EchoSer *echoser = new EchoSer();
	echoser->init();

	WorkerThreadPool *wtp = new	WorkerThreadPool();
	wtp->Start(100);	
	for (int i = 0; i < 2; i++)
	{
		//printf("Add work:%d\n", i);
		wtp->AddWork(echoser);		
	}
	wtp->Shutdown();
	wtp->JoinAll();
	echoser->destroy();
	delete wtp;
	delete echoser;

	return 0;
}
