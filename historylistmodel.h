#ifndef HISTORYLISTMODEL_H
#define HISTORYLISTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QDateTime>

struct Experiment
{
    QString name;
    QString subject;
    QString path;
    QDateTime date;
};

class HistoryListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit HistoryListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex & parent = {}) const override;
    QVariant data(const QModelIndex & index = {}, int role = Qt::DisplayRole) const override;
    Q_INVOKABLE bool removeExperiment(int index);
    Q_INVOKABLE Experiment getExperiment(int index);
    QHash<int, QByteArray> roleNames() const override;

private:
    enum HistoryRole{
        ExperimentRole = Qt::UserRole + 1,
        SubjectRole,
        DateRole
    };

    QList<Experiment> m_experiments;

};

#endif // HISTORYLISTMODEL_H
