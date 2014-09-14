#include <memory>
#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string>
#include <cstring>
#include "socket.h"
#include <pthread.h>

namespace mmtraining {

////////////////////////////////////////////////ClientSocket

ClientSocket::ClientSocket() : fd(-1) {}
    
ClientSocket::ClientSocket(int fd) {
    this->fd = fd;
	if (pthread_mutex_init(&mutex, NULL) != 0)
	{
		printf("pthread_mutex_init fail.\n");
		exit(-1);
	}
}

ClientSocket::~ClientSocket() {
	close(fd);
	//setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, NULL, 0);
    // TODO: 释放资源
}
    
int ClientSocket::Connect(const char* ip, unsigned short port) {
    // TODO: 完成代码
	
	pthread_mutex_lock(&mutex);	
	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)	
	{
		printf("Client socket() failed.\n");
		return -1;
	}

	sockaddr_in clientaddr;
	memset(&clientaddr, 0, sizeof(sockaddr));
	clientaddr.sin_family = AF_INET;
	clientaddr.sin_port = htons(port);
	if (inet_pton(AF_INET, ip, &clientaddr.sin_addr) <= 0)
	{
		printf("inet_pton error for %s\n", ip);	
		return -1;
	}

	if (connect(fd, (sockaddr*)&clientaddr, sizeof(clientaddr)) < 0)
	{
		printf("connect error: %s(errno: %d)\n", strerror(errno), errno);	
		return -1;
	}
	pthread_mutex_unlock(&mutex);	
	
    return 0;
}
    
int ClientSocket::Write(const void* buffer, int bufferSize) {
    // TODO: 完成代码
	
	pthread_mutex_lock(&mutex);
	//printf("write msg\n");
	if (write(fd, buffer, bufferSize) < 0)	
	{
		printf("write msg error: %s(errno: %d)\n", strerror(errno), errno);	
		return -1;
	}
	pthread_mutex_unlock(&mutex);
    return bufferSize;
}

int ClientSocket::WriteLine(const std::string& line) {
    // TODO: 完成代码
	
	pthread_mutex_lock(&mutex);
	//printf("writeLine msg\n");
	if (write(fd, line.c_str(), line.length()) < 0)
	{
		printf("writeLine msg error: %s(errno: %d)\n", strerror(errno), errno);	
		return -1;
	}
	char c = '\n';
	if (line[line.length() - 1] != c)
	{
		if (Write(&c, 1) < 0)	
		{
			printf("writeLine msg error: %s(errno: %d)\n", strerror(errno), errno);	
			return -1;
		}
	}
	pthread_mutex_unlock(&mutex);
    return line.length();
}

int ClientSocket::Read(void* buffer, int bufferSize) {
    // TODO: 完成代码
	
	pthread_mutex_lock(&mutex);
	//printf("read msg\n");
	int length(0);
	if ((length = read(fd ,buffer, bufferSize)) < 0)
	{
		printf("read msg error: %s(errno: %d)\n", strerror(errno), errno);		
		return -1;
	}
	pthread_mutex_unlock(&mutex);
	return length;
}


int ClientSocket::ReadLine(std::string& line) {
	// TODO: 完成代码
	
	pthread_mutex_lock(&mutex);
	printf("readline msg\n");
	char buffer[4096];
	int k = 0;
	do 
	{
		char c;
		if (read(fd, &c, 1) < 0)	
		{
			printf("readLine failed.\n");	
			return -1;
		}
		buffer[k++] = c;
		if (c == '\0')
			break;
		if (c == '\n')
			break;
	} while (1);
	line = buffer;	
	pthread_mutex_unlock(&mutex);

	return k;
}

int ClientSocket::ReadAll(std::string& lines)
{
	pthread_mutex_lock(&mutex);
	//printf("readall msg\n");
	std::string line;
	while ((ReadLine(line)) > 1)
	{
		lines.append(line);	
	}
	pthread_mutex_unlock(&mutex);
	return lines.length();
}

int ClientSocket::Close() {
    // TODO: 完成代码
	close(fd);	
    return -1;
}

////////////////////////////////////////////////ServerSocket

ServerSocket::ServerSocket() : fd(-1) {
    // TODO: 完成代码
	if (pthread_mutex_init(&mutex, NULL) != 0)
	{
		printf("pthread_mutex_init fail.\n");
		exit(-1);
	}
}

ServerSocket::~ServerSocket() {
	//setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, NULL, 0);
	close(fd);
    // TODO: 释放资源
}

int ServerSocket::Listen(const char* ip, unsigned short port) {
    // TODO: 完成代码
	
	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("creat socket error: %s(errno: %d)\n", strerror(errno), errno);
		return -1;
	}

	sockaddr_in servaddr;
	memset((void *)&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	//servaddr.sin_addr.s_addr = htonl(inet_addr(ip));
	servaddr.sin_port = htons(port);
	if (inet_pton(AF_INET, ip, &servaddr.sin_addr) <= 0)
	{
		printf("inet_pton error for %s\n", ip);	
		return -1;
	}
	

	if (bind(fd, (sockaddr*)&servaddr, sizeof(servaddr)) == -1)
	{
		printf("bind socket error: %s (errno: %d)\n", strerror(errno), errno);	
		return -1;
	}
	
	if (listen(fd, 10) == -1)
	{
		printf("listen socket error: %s(errno: %d)\n", strerror(errno), errno);	
		return -1;
	}

    return -1;
}

std::shared_ptr<ClientSocket> ServerSocket::Accept() {
    // TODO: 完成代码

	pthread_mutex_lock(&mutex);	
	int connfd;
	sockaddr_in clientaddr;
	socklen_t addrlen = sizeof(clientaddr);
	memset((void *)&clientaddr, 0, sizeof(clientaddr));
	if ((connfd = accept(fd, (sockaddr*)&clientaddr, &addrlen)) == -1)
	{
		printf("accept socket error: %s(errno: %d)", strerror(errno), errno);	
		return NULL;
	}
	printf("Client's IP:%s:%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	pthread_mutex_unlock(&mutex);
	return std::make_shared<ClientSocket>(connfd);
}

} // namespace mmtraining
