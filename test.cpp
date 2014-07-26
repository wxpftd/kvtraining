#include <iostream>
#include "threadpool.h"
#include <unistd.h>

using namespace std;
using namespace mmtraining;

void* Threadshow(void* arg)
{
	cerr << "Thread " << pthread_self() << " is running." << endl;
	return NULL;
}
int main()
{
		Thread t;
		t.Start();
		t.Join();
	return 0;
}
