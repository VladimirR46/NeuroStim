#include "taskmanager.h"
#include <QFile>
#include<QJsonDocument>
#include <QQmlEngine>
#include "test1task.h"
#include "backgroundactivitytask.h"
#include "pointfindertask.h"

TaskManager::TaskManager(QObject* parent) : QObject(parent)
{
    m_tasks.append(new BackgroundActivityTask());
    m_tasks.append(new PointFinderTask());
    m_tasks.append(new Test1Task());
    //m_tasks.append(QSharedPointer<Test1Task>(new Test1Task));
    //m_tasks.append(QSharedPointer<BackgroundActivityTask>(new BackgroundActivityTask));
    //m_tasks.append(QSharedPointer<PointFinderTask>(new PointFinderTask));
}

TaskManager::~TaskManager()
{
    qDebug()<<"m_tasks.clear()";
    qDeleteAll(m_tasks);
    m_tasks.clear();
}

BaseTask* TaskManager::getTask(size_t index)
{
    return m_tasks.at(index);
}

int TaskManager::taskCount() const
{
    return m_tasks.size();
}

bool TaskManager::loadTask(QString file)
{
    /*
    QSharedPointer<TaskLayout> Task(new TaskLayout());

    if(!Task->load(file))
        return false;

    m_tasks.append(Task);
    */
    return true;
}

Properties *TaskManager::taskProperties(int idx)
{
    return m_tasks.at(idx)->getProperties();
}

TaskListModel::TaskListModel(QObject *parent)
    :QAbstractListModel(parent)
    , m_manager(nullptr)
{
}

int TaskListModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid() || !m_manager)
        return 0;
    return m_manager->taskCount();
}

QVariant TaskListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || !m_manager || index.row() > m_manager->taskCount())
        return QVariant();


    const BaseTask *task = m_manager->getTask(index.row());

    switch(role){
    case TasksRole::NameRole:{
        return QVariant::fromValue(task->name());
    }
    case TasksRole::DescriptionRole:{
        return QVariant::fromValue(task->description());
    }
    default: {
        return true;
    }
    }
}

QHash<int, QByteArray> TaskListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TasksRole::NameRole] = "name";
    roles[TasksRole::DescriptionRole] = "desc";
    return roles;
}

TaskManager *TaskListModel::manager() const
{
    return m_manager;
}

void TaskListModel::setManager(TaskManager *manager)
{
    beginResetModel();

    if(m_manager)
        m_manager->disconnect(this);

    m_manager = manager;

    if(m_manager){
        // here connect events
    }

    endResetModel();
}
