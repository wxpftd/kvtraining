#include <iostream>
#include <unistd.h>
#include <string>
#include <sys/socket.h>
#include "socket.h"


using namespace std;
using namespace mmtraining;

int main()
{
	cout << "This is client." << endl;
	ClientSocket oneClient;
	string clientBuffer{"this is client."};
	oneClient.Connect("127.0.0.1", 6666);
	oneClient.WriteLine(clientBuffer);
	return 0;
}

