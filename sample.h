#ifndef SAMPLE_H
#define SAMPLE_H

#include <QList>
#include "item.h"

namespace Events
{
    enum SwitchType
    {
        TimeOut,
        ItemEvent
    };
}

class SwitchEvent
{
public:
    Events::SwitchType type = Events::TimeOut;
    QList<Item*> items;
    int64_t time = 0;
    int event_counter = 0;
};

class Sample : public QObject
{
    Q_OBJECT
public:
    Sample();
    ~Sample();

    void start(LSL *lsl);
    void stop();
    void setSwitchEvent(Events::SwitchType type, QList<Item*> items);
    void setSwitchEvent(Events::SwitchType type, Item* item);
    void setSwitchEvent(Events::SwitchType type, int64_t time);
    SwitchEvent *getSwitchEvent();
    void switchEvent(Item* item);
    int itemCount();
    Item* getItem(int idx);
    Item* getItem(QString name);
    void addItem(Item* item);
    LSL* lsl();
    void setLslSending(bool value);
    void skipInPreview(bool skip);
    bool isSkip();

    template <typename T>
    T* CreateItem() {
        T *item = new T(this);
        addItem(item);
        return item;
    }



    void setBackgroundСolor(QColor color);
    QColor backgroundСolor();

    void toJson(QJsonObject &json) const;
    void fromJson(const QJsonObject &json);

signals:
    void SwitchSampleSignal(Sample *sample);

private:
    QList<Item*> item_list;
    SwitchEvent switch_event;
    Properties m_properties;
    LSL *m_lsl;
    bool m_lsl_sending = true;
    bool m_skip = false;
};

#endif // SAMPLE_H
