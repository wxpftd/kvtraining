#include <iostream>
#include "threadpool.h"
#include <unistd.h>

using namespace std;
using namespace mmtraining;

int main()
{
	Thread threadType;
	ThreadPool tp;
	tp.Start(10, threadType);
	tp.JoinAll();
	return 0;
}
