
#include "simplekvsvr.h"
#include <iostream>
#include <errno.h>

////////////////////////////////////////////SimpleKVIndex

SimpleKVIndex::SimpleKVIndex() {}

SimpleKVIndex::~SimpleKVIndex() {}

int SimpleKVIndex::Get(const char* key, SimpleKVRecord& item) {
    // TODO: ��ɴ���
    return -1;
}

int SimpleKVIndex::Set(const SimpleKVRecord& item) {
    // TODO: ��ɴ���
    return -1;
}

int SimpleKVIndex::Delete(const char* key) {
    // TODO: ��ɴ���
    return -1;
}

////////////////////////////////////////////SimpleKVStore

SimpleKVStore::SimpleKVStore() {}

SimpleKVStore::~SimpleKVStore() {}

int SimpleKVStore::Init(const char* store) {
    // TODO: ��ɴ���
    return -1;
}

int SimpleKVStore::Load(SimpleKVIndex& index) {
    // TODO: ��ɴ���
    return -1;
}

int SimpleKVStore::Read(SimpleKVRecord& record) {
    // TODO: ��ɴ���
    return -1;
}

int SimpleKVStore::Write(SimpleKVRecord& record) {
    // TODO: ��ɴ���
    return -1;
}

int SimpleKVStore::Compact() {
    // TODO: ��ɴ���
    return -1;
}

////////////////////////////////////////////SimpleKVWork

SimpleKVWork::SimpleKVWork(ClientSocket* socket) : clientSocket(socket) {}

SimpleKVWork::~SimpleKVWork() {
    delete clientSocket;
}

bool SimpleKVWork::NeedDelete() const {
    return true;
}

int SimpleKVWork::DoWork() {
    // TODO: �ͻ���������ѭ��
    return -1;
}

////////////////////////////////////////////SimpleKVServer

int SimpleKVServer::Start(const char* ip, unsigned port, const char* file) {
    // תΪ�ػ�����
    if (InitDaemon() != 0) {
        return -1;
    }

    // �����˿�
    if (serverSocket.Listen(ip, port) != 0) {
        std::cout << "listen on " << ip << ":" << port << " error:"
                  << strerror(errno) << std::endl;
        return -1;
    }
    std::cout << "listening on " << ip << ":" << port << std::endl;
    
    // ��ʼ���־ô洢
    if (store.Init(file) != 0) {
        return -1;
    }
    
    // ��ʼ������
    if (store.Load(index) != 0) {
        return -1;
    }
    
    // �����̳߳�
    if (pool.Start(10) != 0) {
        std::cout << "start thread pool error" << std::endl;
        return -1;
    }
    
    // ���� Accept ѭ��
    while (true) {
        // accept ����
        ClientSocket* socket = serverSocket.Accept();
        if (socket == NULL) {
            std::cout << "accept error: " << strerror(errno) << std::endl;
            return -1;
        }
        
        // ���빤������
        pool.AddWork(new SimpleKVWork(socket));
    }
    
    std::cout << "server shutdown" << std::endl;
    return 0;
}

int SimpleKVServer::InitDaemon() {
    // TODO: ��ɴ���
    return -1;
}

////////////////////////////////////////////main

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cout << "usage: " << argv[0] << " <ip> <port> <file-path>" << std::endl;
        return 1;
    }

    SimpleKVServer server;
    return server.Start(argv[1], (unsigned short)atoi(argv[2]), argv[3]);
}
