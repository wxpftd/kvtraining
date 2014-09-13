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

namespace mmtraining {

////////////////////////////////////////////////ClientSocket

ClientSocket::ClientSocket() : fd(-1) {}
    
ClientSocket::ClientSocket(int fd) {
    this->fd = fd;
}

ClientSocket::~ClientSocket() {
	close(fd);
    // TODO: 释放资源
}
    
int ClientSocket::Connect(const char* ip, unsigned short port) {
    // TODO: 完成代码
	
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
	
    return 0;
}
    
int ClientSocket::Write(const void* buffer, int bufferSize) {
    // TODO: 完成代码
	
	printf("write msg: \n");
	if (write(fd, buffer, bufferSize) < 0)	
	{
		printf("write msg error: %s(errno: %d)\n", strerror(errno), errno);	
		return -1;
	}
    return 0;
}

int ClientSocket::WriteLine(const std::string& line) {
    // TODO: 完成代码
	
	printf("writeLine msg: \n");
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
    return 0;
}

int ClientSocket::Read(void* buffer, int bufferSize) {
    // TODO: 完成代码
	
	printf("read msg:\n");
	if (read(fd ,buffer, bufferSize) < 0)
	{
		printf("read msg error: %s(errno: %d)\n", strerror(errno), errno);		
		return -1;
	}
	return 0;
}


int ClientSocket::ReadLine(std::string& line) {
	// TODO: 完成代码
	
	printf("read msg:\n");
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
		if (c == '\0')
			break;
		if (c == '\n')
			break;
		buffer[k++] = c;
	} while (1);
	line = buffer;	

	return 0;
}

int ClientSocket::Close() {
    // TODO: 完成代码
	close(fd);	
    return -1;
}

////////////////////////////////////////////////ServerSocket

ServerSocket::ServerSocket() : fd(-1) {
    // TODO: 完成代码
}

ServerSocket::~ServerSocket() {
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

ClientSocket* ServerSocket::Accept() {
    // TODO: 完成代码
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
	return new ClientSocket(connfd);
}

} // namespace mmtraining
