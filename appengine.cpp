#include "appengine.h"
#include <QQmlContext>
#include "historylistmodel.h"
#include "mailagent.h"

#ifdef Q_OS_ANDROID
#include <QtCore/private/qandroidextras_p.h>
#include <QCoreApplication>
#include <QJniObject>
#endif

AppEngine::AppEngine(QQuickView *view, QObject *parent)
    : QObject{parent}
{
    setFullScreenMode(); // For Android

    qmlRegisterType<HistoryListModel>("History", 1, 0, "HistoryListModel");
    qmlRegisterType<TaskListModel>("Tasks", 1, 0, "TaskListModel");
    qmlRegisterType<MailAgent>("Mail", 1, 0, "MailAgent");
    qmlRegisterUncreatableType<TaskManager>("Tasks", 1, 0, "TaskManager",QStringLiteral("Create in QML"));
    view->rootContext()->setContextProperty("taskManager", &m_manager);

    qmlRegisterType<PropertyListModel>("Properties", 1, 0, "PropertyListModel");

    view->rootContext()->setContextProperty("appEngine", this);
    view->rootContext()->setContextProperty("Subject", &m_subject);
}

bool AppEngine::checkPermission()
{
    /*
    auto result = QtAndroidPrivate::checkPermission(QString("android.permission.WRITE_EXTERNAL_STORAGE")).then([](
        QtAndroidPrivate::PermissionResult result) {
        return result;
    });
    result.waitForFinished();
    return result.result() != QtAndroidPrivate::Denied;
    */
    #ifdef Q_OS_ANDROID
    auto r = QtAndroidPrivate::checkPermission(QString("android.permission.WRITE_EXTERNAL_STORAGE")).result();
    if(r == QtAndroidPrivate::Denied)
    {
        r = QtAndroidPrivate::requestPermission(QString("android.permission.WRITE_EXTERNAL_STORAGE")).result();
        if (r == QtAndroidPrivate::Denied)
            return false;
    }
    #endif
    return true;
}

void AppEngine::setFullScreenMode()
{
    #ifdef Q_OS_ANDROID
    QNativeInterface::QAndroidApplication::runOnAndroidMainThread([]{
        QJniObject activity = QNativeInterface::QAndroidApplication::context();
        activity.callObjectMethod("getWindow", "()Landroid/view/Window;")
            .callObjectMethod("getDecorView", "()Landroid/view/View;")
            .callMethod<void>("setSystemUiVisibility", "(I)V", 0xffffffff);
    }).waitForFinished();
    #endif
}

void AppEngine::startSelectedTask(int idx)
{
    BaseTask* task = m_manager.getTask(idx);
    if(task) {

        Subject subject;
        subject.Name = m_subject["name"].toString();
        subject.LastName = m_subject["lastName"].toString();
        subject.MiddleName = "";
        subject.Age = m_subject["age"].toString();
        subject.Gender = m_subject["gender"].toString();
        subject.ActiveHand = m_subject["activehand"].toString();

        task->setSubject(subject);
        task->saveProperties();
        m_player.start(task, false);
    }
}

void AppEngine::startPreviewTask(int idx)
{
    BaseTask* task = m_manager.getTask(idx);
    if(task) {
        m_player.start(task, true);
    }
}

