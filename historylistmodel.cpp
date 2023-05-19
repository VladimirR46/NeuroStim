#include "historylistmodel.h"
#include <QStandardPaths>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>

HistoryListModel::HistoryListModel(QObject *parent) : QAbstractListModel(parent)
{
    QString DocumentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

    QDir tasks_dir(DocumentsPath+"/NeuroStim/Experiments/");
    QStringList tasks = tasks_dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (auto task : tasks) {
        QDir experiments_dir(DocumentsPath+"/NeuroStim/Experiments/"+task);
        QStringList experiments = experiments_dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

        for (auto exp_name : experiments) {
            QString file= DocumentsPath+"/NeuroStim/Experiments/"+task+"/"+exp_name+"/Task.json";
            QFile loadFile(file);
            if (!loadFile.open(QIODevice::ReadOnly)) {
                qWarning("Couldn't open save file.");
                return;
            }

            QByteArray saveData = loadFile.readAll();
            QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
            QJsonObject json = loadDoc.object();

            QDateTime data = QDateTime::fromString(json["StartTime"].toString(), "dd.MM.yyyy hh:mm:ss");
            QJsonObject subject_json = json["Subject"].toObject();

            Experiment exp;
            exp.name = task;
            exp.subject = subject_json["Name"].toString()+" "+subject_json["LastName"].toString();
            exp.date = data;
            exp.path = DocumentsPath+"/NeuroStim/Experiments/"+task+"/"+exp_name+"/";
            m_experiments.append(exp);
        }
    }

    // Сортировка массива по дате
    std::sort(m_experiments.begin(), m_experiments.end(), [](const Experiment& e1, const Experiment& e2) {
        return (e1.date > e2.date);
    });
}

int HistoryListModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;
    return m_experiments.size();
}

QVariant HistoryListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    switch(role){
    case HistoryRole::ExperimentRole:{
        return  m_experiments[index.row()].name;
    }
    case HistoryRole::SubjectRole:{
        return m_experiments[index.row()].subject;
    }
    case HistoryRole::DateRole:{
        return m_experiments[index.row()].date.toString("dd.MM.yyyy hh.mm.ss");
    }
    default: {
        return true;
    }
    }
}

bool HistoryListModel::removeExperiment(int index)
{
    if (index < 0 || index >= m_experiments.size()) {
        return false;
    }

    beginRemoveRows(QModelIndex(), index, index);
    QDir(m_experiments[index].path).removeRecursively();
    m_experiments.removeAt(index);
    endRemoveRows();

    return true;
}

Experiment HistoryListModel::getExperiment(int index)
{
    if (index < 0 || index >= m_experiments.size()) return Experiment();

    return m_experiments[index];
}

QHash<int, QByteArray> HistoryListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[HistoryRole::ExperimentRole] = "exprtiment";
    roles[HistoryRole::SubjectRole] = "subject";
    roles[HistoryRole::DateRole] = "datatime";
    return roles;
}
