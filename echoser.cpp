#include <memory>
#include <iostream>
#include "echoser.h"
#include "socket.h"
#include "threadpool.h"
#include <pthread.h>

namespace mmtraining {
	EchoSer::EchoSer()
	{
		if (pthread_mutex_init(&mutex, NULL) < 0)
		{
			printf("pthread_mutex_init fail.\n");	
			exit(-1);
		}
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

	int EchoSer::DoWork()
	{
		pthread_mutex_lock(&mutex);
		std::shared_ptr<ClientSocket> serverClient = server->Accept();	
		std::string buffer;
		serverClient->ReadLine(buffer);
		printf("%s", buffer.c_str());
		pthread_mutex_unlock(&mutex);
		printf("DoWork in EchoSer\n");
		return 0;
	}

	bool EchoSer::NeedDelete() const
	{
		return false;	
	}



}
