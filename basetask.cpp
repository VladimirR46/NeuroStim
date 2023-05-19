#include "basetask.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QLabel>
#include <QVBoxLayout>
#include <QStandardPaths>
#include <QDir>

BaseTask::BaseTask(QString name) : m_name(name)
{
   QString AppConfigPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);

   PropertiesDir = AppConfigPath+"/Tasks/";
   QDir dir(PropertiesDir);
   if (!dir.exists()) dir.mkpath(PropertiesDir);
}

BaseTask::~BaseTask()
{
}

bool BaseTask::start(QSize screen)
{
    bool initState = Init(screen);
    m_startDateTime = QDateTime::currentDateTime();
    return initState;
}

void BaseTask::close()
{
    qDeleteAll(m_samples);
    m_samples.clear();
}

QString BaseTask::name() const
{
    return m_name;
}

QString BaseTask::description() const
{
    return m_description;
}

Sample *BaseTask::CreateSample()
{
    m_samples.push_back(new Sample());
    return m_samples.back();
}

int BaseTask::sampleCount()
{
    return m_samples.size();
}

Sample *BaseTask::getSample(int idx)
{
    return m_samples[idx];
}

Properties *BaseTask::getProperties()
{
    return &m_properties;
}

void BaseTask::saveProperties()
{
    QFile saveFile(PropertiesDir+m_name+".json");
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save properties file.");
        return;
    }

    QJsonObject taskObject;
    m_properties.toJson(taskObject);
    saveFile.write(QJsonDocument(taskObject).toJson());
}

bool BaseTask::loadProperties()
{
    QString file= PropertiesDir+m_name+".json";
    QFile loadFile(file);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    QJsonObject json = loadDoc.object();

    m_properties.fromJson(json);
    return true;
}

void BaseTask::setSubject(Subject subject)
{
    m_subject = subject;
}

Subject BaseTask::subject()
{
    return m_subject;
}

bool BaseTask::save(QString file)
{
    if(m_samples.size() == 0 || file.isEmpty())
        return false;

    QJsonObject taskObject;
    taskObject["Version"] = "1.0.0";
    taskObject["TaskName"] = m_name;
    taskObject["Description"] = m_description;
    taskObject["StartTime"] = m_startDateTime.toString("dd.MM.yyyy hh:mm:ss");
    m_properties.toJson(taskObject);
    m_subject.toJson(taskObject);

    QJsonArray samples_json;
    for (auto sample: m_samples) {
        QJsonObject sample_json;
        sample->toJson(sample_json);
        samples_json.append(sample_json);
    }

    taskObject["samples"] = samples_json;

    QFile saveFile(file);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    saveFile.write(QJsonDocument(taskObject).toJson());
    return true;
}

bool BaseTask::load(QString file)
{
    QFile loadFile(file);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    QJsonObject json = loadDoc.object();

    if (!json.contains("TaskName")) return false;
    m_name = json["TaskName"].toString();
    m_description = json["Description"].toString();

    if (json.contains("properties") && json["properties"].isArray()){
        m_properties.fromJson(json);
    }

    if (json.contains("samples") && json["samples"].isArray()){
        QJsonArray samples_json = json["samples"].toArray();

        for(const QJsonValue &v : samples_json) {
            QJsonObject sample_json = v.toObject();

            Sample* sample = CreateSample();
            sample->fromJson(sample_json);
        }
    }

    return true;
}

void BaseTask::gen_sequence(std::vector<int> &v, int begin, int end)
{
    for(int i = 0; i < v.size(); i++)
        v[i] = begin + (i % end);
}

void Subject::toJson(QJsonObject &json) const
{
    QJsonObject subject;
    subject["LastName"] = LastName;
    subject["Name"] = Name;
    subject["MiddleName"] = MiddleName;
    subject["Gender"] = Gender;
    subject["ActiveHand"] = ActiveHand;
    subject["Age"] = Age;
    json["Subject"] = subject;
}
