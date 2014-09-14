#include <memory>
#include <iostream>
#include "echoser.h"
#include "socket.h"
#include "runnable.h"

namespace mmtraining {
	EchoSer::EchoSer()
	{

	}

	EchoSer::~EchoSer()
	{

	}		


	int EchoSer::init()
	{
		server = new ServerSocket();
		server->Listen("127.0.0.1", 6666);
		return 0;
	}

	int EchoSer::destroy()
	{
		delete server;
		return 0;
	}

	int EchoSer::Run()
	{
		std::shared_ptr<ClientSocket> serverClient = server->Accept();	
		std::string buffer;
		serverClient->ReadAll(buffer);
		printf("%s", buffer.c_str());
		return 0;
	}

}
