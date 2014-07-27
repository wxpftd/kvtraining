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

int main()
{
	MyWork myWork;
	WorkQueue wq;
	WorkerThreadPool whp;
	for (int i=0; i<50; i++)
		whp.AddWork(&myWork);
	whp.Start(10);
	//whp.JoinAll();
	//whp.Shutdown();
	return 0;
}
