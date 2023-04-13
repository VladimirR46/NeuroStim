#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QObject>
#include <QList>
#include <QAbstractListModel>
#include "basetask.h"

class TaskManager : public QObject
{
    Q_OBJECT
public:
    explicit TaskManager(QObject* parent = nullptr);
    ~TaskManager();

    BaseTask *getTask(size_t index);
    int taskCount() const;
    bool loadTask(QString file);

    Q_INVOKABLE Properties *taskProperties(int idx);

private:
    QList<BaseTask*> m_tasks;
};

/////////////////////////////////////////////////////////////
/// \brief The TaskListModel class
/////////////////////////////////////////////////////////////

class TaskListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(TaskManager *manager READ manager WRITE setManager)

public:
    explicit TaskListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex & parent = {}) const override;
    QVariant data(const QModelIndex & index = {}, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    TaskManager *manager() const;
    void setManager(TaskManager *manager);

private:
    TaskManager *m_manager;

    enum TasksRole{
        NameRole = Qt::UserRole + 1,
        DescriptionRole
    };

};

#endif // TASKMANAGER_H
