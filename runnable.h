
#ifndef MM_TRAINING_RUNNABLE_H
#define MM_TRAINING_RUNNABLE_H

namespace mmtraining {

/**
 * �����߼�
 */
class Runnable {
public:
    /**
     * ��������
     */
    virtual ~Runnable() {}

    /**
     * �����߼�
     */
    virtual int Run() = 0;
};

}  // mmtraining

#endif
