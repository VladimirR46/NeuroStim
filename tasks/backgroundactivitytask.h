#ifndef BACKGROUNDACTIVITYTASK_H
#define BACKGROUNDACTIVITYTASK_H

#include "../basetask.h"

class BackgroundActivityTask : public BaseTask
{
public:
    BackgroundActivityTask();
    ~BackgroundActivityTask();

    bool Init(QSize screen) override;
};

#endif // BACKGROUNDACTIVITYTASK_H
