#include <iostream>
#include "threadpool.h"
#include <unistd.h>

using namespace std;
using namespace mmtraining;

class MyWork : public Work
{
public:
	bool NeedDelete() const
	{
		return false;	
	}

	int DoWork()
	{
		printf("MyWork is running.\n");	
		//cout << "MyWork is running." << endl;
		return 0;
	}
};

class MyRun : public Runnable
{
public:
	int Run()
	{
		printf("MyRun is working.\n");	
		return 0;
	}
		
};

int main()
{
	MyWork myWork;
	WorkerThreadPool whp;
	for (int i=0; i<1000000; i++)
		whp.AddWork(&myWork);
	whp.Start(100);
	cout << whp.Shutdown() << endl;
	whp.JoinAll();
	return 0;
}
