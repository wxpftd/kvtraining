#include <iostream>
#include "threadpool.h"
#include <unistd.h>
#include <cstdio>

using namespace std;
using namespace mmtraining;

class MyWork : public Work
{
public:
	double *testArea; 
	MyWork()
	{
		testArea = new double[10000];
	}
	~MyWork()
	{
		delete testArea;	
	}
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
	WorkerThreadPool whp;
	long long workCount = 0;
	whp.Start(100);
	while(1)
	//for (int i=0; i<100000; i++)
	{
		MyWork *myWork = new MyWork();
		whp.AddWork(myWork);
		workCount ++;
		cout << workCount << endl;
	}
	cout << whp.Shutdown() << endl;
	whp.JoinAll();
	return 0;
}
