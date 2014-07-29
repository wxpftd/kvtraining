#include <iostream>
#include <unistd.h>
#include "processpool.h"

using namespace std;
using namespace mmtraining;

class MyRun : public Runnable
{
public:
	int Run()
	{
		printf("MyRun is working.\n");	
		exit(0);
	}
		
};

class MyProcess : public Process
{
public:
	int DoRun()
	{
		printf("MyProcess is running.\n");	
		exit(0);
	}
};

int main()
{
	ProcessPool pp;
	MyRun myRun;
	pp.Start(3, myRun);
	cout << "pp.KillAll():" <<  pp.KillAll() << endl;
	cout << "pp.WaitAll():" <<  pp.WaitAll() << endl;
	return 0;
}
