#ifndef POINTFINDERTASK_H
#define POINTFINDERTASK_H

#include "../basetask.h"

class PointFinderTask : public BaseTask
{
public:
    PointFinderTask();
    ~PointFinderTask();

    bool Init(QSize screen) override;

    QList<QPointF> generateRandomPoints(QSize screen);
};

#endif // POINTFINDERTASK_H
