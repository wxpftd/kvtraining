
#ifndef MM_TRAINING_SOCKET_H
#define MM_TRAINING_SOCKET_H

#include <string>
#include <memory>
#include <pthread.h>

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
	 * �������з���˴���������������
	 * @return -1: ����ʧ��, ����: �����ֽ�
	 */
	int ReadAll(std::string& lines);
    
    /**
     * �ر�����
     */
    int Close();
    
private:
	pthread_mutex_t mutex;
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
	std::shared_ptr<ClientSocket> Accept();
    
private:
    int fd;
	pthread_mutex_t mutex;
};

} // namespace mmtraining

#endif // MM_TRAINING_SOCKET_H
