#ifndef TEST1TASK_H
#define TEST1TASK_H

#include "../basetask.h"

class Test1Task : public BaseTask
{
public:
    Test1Task();
    ~Test1Task();

    bool Init(QSize screen) override;
};

#endif // TEST1TASK_H
