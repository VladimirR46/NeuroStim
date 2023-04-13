#ifndef APPENGINE_H
#define APPENGINE_H

#include <QObject>
#include <QQuickView>
#include <QQmlPropertyMap>
#include "taskmanager.h"
#include "properties.h"
#include "player.h"

class AppEngine : public QObject
{
    Q_OBJECT
public:
    explicit AppEngine(QQuickView *view, QObject *parent = nullptr);

    bool checkPermission();
    void setFullScreenMode();
    Q_INVOKABLE void startSelectedTask(int idx);

signals:


private:
    TaskManager m_manager;
    Player m_player;
    QQmlPropertyMap m_subject;
};

#endif // APPENGINE_H
