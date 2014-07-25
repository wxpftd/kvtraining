
#include "processpool.h"
#include <iostream>

using namespace mmtraining;

/**
 * ������Task
 */
class TestTask : public Task {
public:
    /**
     * �������߼�
     * @return 0 ����ɹ�, -1 ����ʧ��
     */
    virtual int DoTask() {
        return -1;
    }

    /**
     * ���л�
     */
    virtual int ToBuffer(std::string& buffer) {
        return -1;
    }

    /**
     * �����л�
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
