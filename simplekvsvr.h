
#ifndef MM_TRANING_SIMPLE_KV_H
#define MM_TRANING_SIMPLE_KV_H

#include "socket.h"
#include "threadpool.h"

using mmtraining::Work;
using mmtraining::ClientSocket;
using mmtraining::ServerSocket;
using mmtraining::WorkerThreadPool;

/**
 * 存储记录
 */
struct SimpleKVRecord {
    char* key;        /**< key */
    char* value;      /**< NULL: 不在内存中, 需要读文件. 非NULL: cache在内存中 */
    
    off_t offset;     /**< value 在文件中的偏移量 */
    size_t length;    /**< value 长度 */
};

/**
 * simple kv 索引, 包含所有key,
 * 同时维护了最近访问记录的value的cache
 */
class SimpleKVIndex {
public:
    /**
     * 构造函数
     */
    SimpleKVIndex();

    /**
     * 析构函数
     */
    ~SimpleKVIndex();

    /**
     * 获取 value
     * @return 0: 成功, 1: key 不存在, -1: 失败
     */
    int Get(const char* key, SimpleKVRecord& item);

    /**
     * 设置 key-value
     */
    int Set(const SimpleKVRecord& item);

    /**
     * 删除 key-value
     * @return 0: 删除成功, 1: key 不存在, -1: 失败
     */
    int Delete(const char* key);
    
private:
    
};

/**
 * simple kv 存储, 使用 append 写
 */
class SimpleKVStore {
public:
    /**
     * 构造函数
     */
    SimpleKVStore();

    /**
     * 析构函数
     */
    ~SimpleKVStore();

    /**
     * 初始化存储文件
     */
    int Init(const char* store);

    /**
     * 将持久存储加载到index
     */
    int Load(SimpleKVIndex& index);

    /**
     * 读数据
     * @param record 需传入 offset 和 length,
     *               读取完毕后, 输出 value
     * @return 0: 成功, -1: 失败
     */
    int Read(SimpleKVRecord& record);

    /**
     * 写数据
     * @param record 需传入 key, value 和 length
     *               写入完毕后, 输出 offset
     * @return 0: 成功, -1: 失败
     */
    int Write(SimpleKVRecord& record);

    /**
     * 剔除已删除记录, 重新生成存储
     */
    int Compact();
    
private:

};

/**
 * simple kv 处理逻辑
 *
 * 协议(- 请求, + 响应):
 *
 * 1) 获取 key 对应的 value
 * -get <key>\n
 * +<value>\n
 *
 * 2) 设置 key-value(存在则覆盖, 否则新增)
 * -set <key> <value>\n
 * +OK\n
 *
 * 3) 删除 key-value
 * -delete <key>\n
 * +OK\n
 * 
 * 4) 统计
 * -stats\n
 * +count: <key-count>, mem: <mem-size>, file: <file-size>,
 *  hits: <hit-count>, misses: <miss-count>\n
 *
 * 5) 退出
 * -quit\n
 * +OK\n
 */
class SimpleKVWork : public Work {
public:
    /**
     * 构造函数
     * @param socket 客户端连接
     */
    SimpleKVWork(ClientSocket* socket);
    
    /**
     * 析构函数
     */
    ~SimpleKVWork();

    /**
     * 是否需要被 delete
     */
    virtual bool NeedDelete() const;

    /**
     * 工作处理逻辑
     * @return 0 处理成功, -1 处理失败
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
     * 启动server
     */
    int Start(const char* ip, unsigned port, const char* file);

private:

    /**
     * 转为守护进程
     */
    int InitDaemon();

    SimpleKVIndex index;
    SimpleKVStore store;
    ServerSocket serverSocket;
    WorkerThreadPool pool;
};

#endif // MM_TRANNING_SIMPLE_KV_H
