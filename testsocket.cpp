
#include "socket.h"
#include "threadpool.h"
#include <memory>
#include <iostream>
#include <errno.h>

using namespace mmtraining;

/**
 * server
 */
class Server : public Thread {
public:

    int Start(unsigned short port) {
        if (serverSocket.Listen("127.0.0.1", port) != 0) {
            std::cout << "Test ERROR: Listen error, " << strerror(errno) << std::endl;
            return -1;
        }
        return Thread::Start();
    }
    
private:
    
    int DoRun() {
        std::auto_ptr<ClientSocket> client(serverSocket.Accept());
        if (client.get() == NULL) {
            std::cout << "Test ERROR: Accept error, " << strerror(errno) << std::endl;
            return -1;
        }
        
        std::string data;
        if (client->ReadLine(data) == -1) {
            std::cout << "Test ERROR: ReadLine error, " << strerror(errno) << std::endl;
            return -1;
        }
        
        if (data != "123") {
            std::cout << "Test ERROR: data error, " << data << std::endl;
            return -1;
        }
        
        std::cout << "Test OK" << std::endl;
        return 0;
    }
    
    ServerSocket serverSocket;
};

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }
    
    unsigned short port = (unsigned short)atoi(argv[1]);
    
    Server server;
    if (server.Start(port) != 0) {
        return 1;
    }
    
    ClientSocket client;
    if (client.Connect("127.0.0.1", port) != 0) {
        std::cout << "Test ERROR: Connect error, " << strerror(errno) << std::endl;
        return -1;
    }
    
    if (client.WriteLine("123") == -1) {
        std::cout << "Test ERROR: WriteLine error, " << strerror(errno) << std::endl;
        return -1;
    }
    
    server.Join();
    
    return 0;
}
