#include <memory>
#include <iostream>
#include "echoser.h"
#include "socket.h"
#include "threadpool.h"
#include <pthread.h>
#include <cstring>

namespace mmtraining {
	EchoSer::EchoSer()
	{
		//if (pthread_mutex_init(&mutex, NULL) < 0)
		//{
		//	printf("pthread_mutex_init fail.\n");	
		//	exit(-1);
		//}
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
		std::shared_ptr<ClientSocket> serverClient = server->Accept();	
		//pthread_mutex_lock(&mutex);
		char buffer[4096]{};
		while(strcmp(buffer, "exit\n"))	
		{
			memset(buffer, '\0', sizeof(buffer));
			serverClient->Read(buffer, 4096);
			printf("%s", buffer);
		}
		//pthread_mutex_unlock(&mutex);
		//printf("DoWork in EchoSer\n");
		return 0;
	}

	bool EchoSer::NeedDelete() const
	{
		return false;	
	}

}
