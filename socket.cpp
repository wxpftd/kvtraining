
#include "socket.h"

namespace mmtraining {

////////////////////////////////////////////////ClientSocket

ClientSocket::ClientSocket() : fd(-1) {}
    
ClientSocket::ClientSocket(int fd) {
    this->fd = fd;
}

ClientSocket::~ClientSocket() {
    // TODO: �ͷ���Դ
}
    
int ClientSocket::Connect(const char* ip, unsigned short port) {
    // TODO: ��ɴ���
    return -1;
}
    
int ClientSocket::Write(const void* buffer, int bufferSize) {
    // TODO: ��ɴ���
    return -1;
}

int ClientSocket::WriteLine(const std::string& line) {
    // TODO: ��ɴ���
    return -1;
}

int ClientSocket::Read(void* buffer, int bufferSize) {
    // TODO: ��ɴ���
    return -1;
}

int ClientSocket::ReadLine(std::string& line) {
    // TODO: ��ɴ���
    return -1;
}

int ClientSocket::Close() {
    // TODO: ��ɴ���
    return -1;
}

////////////////////////////////////////////////ServerSocket

ServerSocket::ServerSocket() : fd(-1) {
    // TODO: ��ɴ���
}

ServerSocket::~ServerSocket() {
    // TODO: �ͷ���Դ
}

int ServerSocket::Listen(const char* ip, unsigned short port) {
    // TODO: ��ɴ���
    return -1;
}

ClientSocket* ServerSocket::Accept() {
    // TODO: ��ɴ���
    return NULL;
}

} // namespace mmtraining
