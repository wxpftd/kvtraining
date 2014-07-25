
#include "socket.h"

namespace mmtraining {

////////////////////////////////////////////////ClientSocket

ClientSocket::ClientSocket() : fd(-1) {}
    
ClientSocket::ClientSocket(int fd) {
    this->fd = fd;
}

ClientSocket::~ClientSocket() {
    // TODO: 释放资源
}
    
int ClientSocket::Connect(const char* ip, unsigned short port) {
    // TODO: 完成代码
    return -1;
}
    
int ClientSocket::Write(const void* buffer, int bufferSize) {
    // TODO: 完成代码
    return -1;
}

int ClientSocket::WriteLine(const std::string& line) {
    // TODO: 完成代码
    return -1;
}

int ClientSocket::Read(void* buffer, int bufferSize) {
    // TODO: 完成代码
    return -1;
}

int ClientSocket::ReadLine(std::string& line) {
    // TODO: 完成代码
    return -1;
}

int ClientSocket::Close() {
    // TODO: 完成代码
    return -1;
}

////////////////////////////////////////////////ServerSocket

ServerSocket::ServerSocket() : fd(-1) {
    // TODO: 完成代码
}

ServerSocket::~ServerSocket() {
    // TODO: 释放资源
}

int ServerSocket::Listen(const char* ip, unsigned short port) {
    // TODO: 完成代码
    return -1;
}

ClientSocket* ServerSocket::Accept() {
    // TODO: 完成代码
    return NULL;
}

} // namespace mmtraining
