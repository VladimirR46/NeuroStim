#ifndef BASETASK_H
#define BASETASK_H

#include "sample.h"
#include <QList>
#include <random>
#include <QFile>

struct Subject
{
   QString LastName;
   QString Name;
   QString MiddleName;
   QString Gender;
   QString ActiveHand;
   QString Age;

   void toJson(QJsonObject &json) const;
};

class BaseTask
{
public:
    BaseTask(QString name);
    virtual ~BaseTask();

    bool start(QSize screen);
    virtual bool Init(QSize screen) = 0;
    void close();

    QString name() const;
    QString description() const;
    Sample *CreateSample();
    int sampleCount();
    Sample* getSample(int idx);
    Properties *getProperties();
    void saveProperties();
    bool loadProperties();

    void setSubject(Subject subject);
    Subject subject();
    bool save(QString file);
    bool load(QString file);

    //utils
    void gen_sequence(std::vector<int> &v, int begin, int end);

protected:
    QString m_name;
    Properties m_properties;
    QString m_description;
    QString PropertiesDir;

    //random
    std::random_device rd;
    std::mt19937 g{rd()};

private:
    Subject m_subject;
    QDateTime m_startDateTime;
    QList<Sample*> m_samples;
};

///////////////////////////////////////////////////////////////////////////////
/// \brief The TaskLayout class
///////////////////////////////////////////////////////////////////////////////

class TaskLayout : public BaseTask
{
public:
    TaskLayout() : BaseTask("Layout") {}
    bool Init(QSize screen) override { return true; }
};

#endif // BASETASK_H
