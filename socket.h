
#ifndef MM_TRAINING_SOCKET_H
#define MM_TRAINING_SOCKET_H

#include <string>

namespace mmtraining {

/**
 * 客户端 socket
 */
class ClientSocket {
public:
    /**
     * 初始化
     */
    ClientSocket();
    
    /**
     * 初始化
     */
    ClientSocket(int fd);

    /**
     * 析构函数
     */
    ~ClientSocket();
    
    /**
     * 连接服务器
     */
    int Connect(const char* ip, unsigned short port);
    
    /**
     * 发送数据
     * @return -1: 发送失败, 其他: 发送字节数
     */
    int Write(const void* buffer, int bufferSize);
    
    /**
     * 发送数据, 自动补全换行符
     * @return -1: 发送失败, 其他: 发送字节数
     */
    int WriteLine(const std::string& line);
    
    /**
     * 接收数据
     * @return -1: 接收失败, 其他: 接收字节数
     */
    int Read(void* buffer, int bufferSize);
    
    /**
     * 接收一行数据
     * @return -1: 接收失败, 其他: 接收字节数
     */
    int ReadLine(std::string& line);
    
    /**
     * 关闭连接
     */
    int Close();
    
private:
    
    int fd;
};

/**
 * 服务端 socket
 */
class ServerSocket {
public:
    /**
     * 构造函数
     */
    ServerSocket();

    /**
     * 析构函数
     */
    ~ServerSocket();
    
    /**
     * 监听端口
     */
    int Listen(const char* ip, unsigned short port);
    
    /**
     * accept 新连接
     */
    ClientSocket* Accept();
    
private:
    int fd;
};

} // namespace mmtraining

#endif // MM_TRAINING_SOCKET_H
