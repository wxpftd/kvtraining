
#ifndef MM_TRANING_SIMPLE_KV_H
#define MM_TRANING_SIMPLE_KV_H

#include "socket.h"
#include "threadpool.h"

using mmtraining::Work;
using mmtraining::ClientSocket;
using mmtraining::ServerSocket;
using mmtraining::WorkerThreadPool;

/**
 * �洢��¼
 */
struct SimpleKVRecord {
    char* key;        /**< key */
    char* value;      /**< NULL: �����ڴ���, ��Ҫ���ļ�. ��NULL: cache���ڴ��� */
    
    off_t offset;     /**< value ���ļ��е�ƫ���� */
    size_t length;    /**< value ���� */
};

/**
 * simple kv ����, ��������key,
 * ͬʱά����������ʼ�¼��value��cache
 */
class SimpleKVIndex {
public:
    /**
     * ���캯��
     */
    SimpleKVIndex();

    /**
     * ��������
     */
    ~SimpleKVIndex();

    /**
     * ��ȡ value
     * @return 0: �ɹ�, 1: key ������, -1: ʧ��
     */
    int Get(const char* key, SimpleKVRecord& item);

    /**
     * ���� key-value
     */
    int Set(const SimpleKVRecord& item);

    /**
     * ɾ�� key-value
     * @return 0: ɾ���ɹ�, 1: key ������, -1: ʧ��
     */
    int Delete(const char* key);
    
private:
    
};

/**
 * simple kv �洢, ʹ�� append д
 */
class SimpleKVStore {
public:
    /**
     * ���캯��
     */
    SimpleKVStore();

    /**
     * ��������
     */
    ~SimpleKVStore();

    /**
     * ��ʼ���洢�ļ�
     */
    int Init(const char* store);

    /**
     * ���־ô洢���ص�index
     */
    int Load(SimpleKVIndex& index);

    /**
     * ������
     * @param record �贫�� offset �� length,
     *               ��ȡ��Ϻ�, ��� value
     * @return 0: �ɹ�, -1: ʧ��
     */
    int Read(SimpleKVRecord& record);

    /**
     * д����
     * @param record �贫�� key, value �� length
     *               д����Ϻ�, ��� offset
     * @return 0: �ɹ�, -1: ʧ��
     */
    int Write(SimpleKVRecord& record);

    /**
     * �޳���ɾ����¼, �������ɴ洢
     */
    int Compact();
    
private:

};

/**
 * simple kv �����߼�
 *
 * Э��(- ����, + ��Ӧ):
 *
 * 1) ��ȡ key ��Ӧ�� value
 * -get <key>\n
 * +<value>\n
 *
 * 2) ���� key-value(�����򸲸�, ��������)
 * -set <key> <value>\n
 * +OK\n
 *
 * 3) ɾ�� key-value
 * -delete <key>\n
 * +OK\n
 * 
 * 4) ͳ��
 * -stats\n
 * +count: <key-count>, mem: <mem-size>, file: <file-size>,
 *  hits: <hit-count>, misses: <miss-count>\n
 *
 * 5) �˳�
 * -quit\n
 * +OK\n
 */
class SimpleKVWork : public Work {
public:
    /**
     * ���캯��
     * @param socket �ͻ�������
     */
    SimpleKVWork(ClientSocket* socket);
    
    /**
     * ��������
     */
    ~SimpleKVWork();

    /**
     * �Ƿ���Ҫ�� delete
     */
    virtual bool NeedDelete() const;

    /**
     * ���������߼�
     * @return 0 ����ɹ�, -1 ����ʧ��
     */
    virtual int DoWork();

private:
    ClientSocket* clientSocket;
};

/**
 * simple kv server
 */
class SimpleKVServer {
public:

    /**
     * ����server
     */
    int Start(const char* ip, unsigned port, const char* file);

private:

    /**
     * תΪ�ػ�����
     */
    int InitDaemon();

    SimpleKVIndex index;
    SimpleKVStore store;
    ServerSocket serverSocket;
    WorkerThreadPool pool;
};

#endif // MM_TRANNING_SIMPLE_KV_H
