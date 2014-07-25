
#ifndef MM_TRAINING_SOCKET_H
#define MM_TRAINING_SOCKET_H

#include <string>

namespace mmtraining {

/**
 * �ͻ��� socket
 */
class ClientSocket {
public:
    /**
     * ��ʼ��
     */
    ClientSocket();
    
    /**
     * ��ʼ��
     */
    ClientSocket(int fd);

    /**
     * ��������
     */
    ~ClientSocket();
    
    /**
     * ���ӷ�����
     */
    int Connect(const char* ip, unsigned short port);
    
    /**
     * ��������
     * @return -1: ����ʧ��, ����: �����ֽ���
     */
    int Write(const void* buffer, int bufferSize);
    
    /**
     * ��������, �Զ���ȫ���з�
     * @return -1: ����ʧ��, ����: �����ֽ���
     */
    int WriteLine(const std::string& line);
    
    /**
     * ��������
     * @return -1: ����ʧ��, ����: �����ֽ���
     */
    int Read(void* buffer, int bufferSize);
    
    /**
     * ����һ������
     * @return -1: ����ʧ��, ����: �����ֽ���
     */
    int ReadLine(std::string& line);
    
    /**
     * �ر�����
     */
    int Close();
    
private:
    
    int fd;
};

/**
 * ����� socket
 */
class ServerSocket {
public:
    /**
     * ���캯��
     */
    ServerSocket();

    /**
     * ��������
     */
    ~ServerSocket();
    
    /**
     * �����˿�
     */
    int Listen(const char* ip, unsigned short port);
    
    /**
     * accept ������
     */
    ClientSocket* Accept();
    
private:
    int fd;
};

} // namespace mmtraining

#endif // MM_TRAINING_SOCKET_H
