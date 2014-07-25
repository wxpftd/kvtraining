
#include "simplekvsvr.h"
#include <iostream>
#include <errno.h>

////////////////////////////////////////////SimpleKVIndex

SimpleKVIndex::SimpleKVIndex() {}

SimpleKVIndex::~SimpleKVIndex() {}

int SimpleKVIndex::Get(const char* key, SimpleKVRecord& item) {
    // TODO: 完成代码
    return -1;
}

int SimpleKVIndex::Set(const SimpleKVRecord& item) {
    // TODO: 完成代码
    return -1;
}

int SimpleKVIndex::Delete(const char* key) {
    // TODO: 完成代码
    return -1;
}

////////////////////////////////////////////SimpleKVStore

SimpleKVStore::SimpleKVStore() {}

SimpleKVStore::~SimpleKVStore() {}

int SimpleKVStore::Init(const char* store) {
    // TODO: 完成代码
    return -1;
}

int SimpleKVStore::Load(SimpleKVIndex& index) {
    // TODO: 完成代码
    return -1;
}

int SimpleKVStore::Read(SimpleKVRecord& record) {
    // TODO: 完成代码
    return -1;
}

int SimpleKVStore::Write(SimpleKVRecord& record) {
    // TODO: 完成代码
    return -1;
}

int SimpleKVStore::Compact() {
    // TODO: 完成代码
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
    // TODO: 客户端请求处理循环
    return -1;
}

////////////////////////////////////////////SimpleKVServer

int SimpleKVServer::Start(const char* ip, unsigned port, const char* file) {
    // 转为守护进程
    if (InitDaemon() != 0) {
        return -1;
    }

    // 监听端口
    if (serverSocket.Listen(ip, port) != 0) {
        std::cout << "listen on " << ip << ":" << port << " error:"
                  << strerror(errno) << std::endl;
        return -1;
    }
    std::cout << "listening on " << ip << ":" << port << std::endl;
    
    // 初始化持久存储
    if (store.Init(file) != 0) {
        return -1;
    }
    
    // 初始化索引
    if (store.Load(index) != 0) {
        return -1;
    }
    
    // 启动线程池
    if (pool.Start(10) != 0) {
        std::cout << "start thread pool error" << std::endl;
        return -1;
    }
    
    // 进行 Accept 循环
    while (true) {
        // accept 连接
        ClientSocket* socket = serverSocket.Accept();
        if (socket == NULL) {
            std::cout << "accept error: " << strerror(errno) << std::endl;
            return -1;
        }
        
        // 加入工作队列
        pool.AddWork(new SimpleKVWork(socket));
    }
    
    std::cout << "server shutdown" << std::endl;
    return 0;
}

int SimpleKVServer::InitDaemon() {
    // TODO: 完成代码
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
