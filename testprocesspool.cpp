
#include "processpool.h"
#include <iostream>

using namespace mmtraining;

/**
 * 测试用Task
 */
class TestTask : public Task {
public:
    /**
     * 任务处理逻辑
     * @return 0 处理成功, -1 处理失败
     */
    virtual int DoTask() {
        return -1;
    }

    /**
     * 序列化
     */
    virtual int ToBuffer(std::string& buffer) {
        return -1;
    }

    /**
     * 反序列化
     */
    virtual int FromBuffer(std::string& buffer) {
        return -1;
    }
};

int main(int argc, char** argv) {
    TestTask taskType;
    TaskProcessPool pool(taskType);

    if (pool.Start(2) != 0) {
        std::cout << "Test Start ERROR" << std::endl;
        return 1;
    }

    if (pool.KillAll() != 0) {
        std::cout << "Test Start ERROR" << std::endl;
        return 2;
    }

    return 0;
}
