#include <iostream>
#include "threadpool.h"

using namespace std;
using namespace mmtraining;

int main()
{
	Thread t;
	t.Start();	
	t.Join();
	return 0;
}
