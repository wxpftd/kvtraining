#include <iostream>
#include <unistd.h>
#include <string>
#include <sys/socket.h>
#include "socket.h"

using namespace std;
using namespace mmtraining;

int main()
{
	cout << "This is server." << endl;
	ServerSocket server;
	string serverBuffer;
	server.Listen("127.0.0.1", 6666);
	shared_ptr<ClientSocket> serverClient = server.Accept();
	
	serverClient->ReadLine(serverBuffer);
	cout << serverBuffer;
	//serverBuffer.append("--Server get msg");
	//serverClient->WriteLine(serverBuffer);

	return 0;
}
