#include "sample.h"
#include <QJsonObject>

Sample::Sample()
{
    m_properties.add(Property("backgroundСolor",QColor(Qt::white),"Цвет фона",UIType::ColorDialog));
}

Sample::~Sample()
{
    item_list.clear();
}

void Sample::start(LSL *lsl)
{
    m_lsl = lsl;
    if(m_lsl_sending) m_lsl->sendEvent(Event("Sample", 0, "begin"));
}

void Sample::stop()
{
    if(m_lsl_sending) m_lsl->sendEvent(Event("Sample", 0, "end"));
}

void Sample::setSwitchEvent(Events::SwitchType type, QList<Item *> items)
{
    switch_event.type = type;
    switch_event.items = items;
}

void Sample::setSwitchEvent(Events::SwitchType type, Item *item)
{
    switch_event.type = type;
    switch_event.items.push_back(item);
}

void Sample::setSwitchEvent(Events::SwitchType type, int64_t time)
{
    switch_event.type = type;
    switch_event.time = time;
}

SwitchEvent *Sample::getSwitchEvent()
{
    return &switch_event;
}

void Sample::switchEvent(Item *item)
{
    if(switch_event.items.contains(item))
        switch_event.event_counter++;

    if(switch_event.event_counter==switch_event.items.count()) {
        emit SwitchSampleSignal(this);
    }
}

int Sample::itemCount()
{
    return item_list.size();
}

Item* Sample::getItem(int idx)
{
    return item_list[idx];
}

Item *Sample::getItem(QString name)
{
    for(int i = 0; i < item_list.size(); i++)
        if(name == item_list[i]->name())
            return item_list[i];
    return nullptr;
}

LSL *Sample::lsl()
{
    return m_lsl;
}

void Sample::setLslSending(bool value)
{
    m_lsl_sending = value;
}

void Sample::skipInPreview(bool skip)
{
    m_skip = skip;
}

bool Sample::isSkip()
{
    return m_skip;
}

void Sample::addItem(Item* item)
{
    item->setId(item_list.size());
    item_list.append(item);
}

void Sample::setBackgroundСolor(QColor color)
{
    m_properties["backgroundСolor"] = color;
}

QColor Sample::backgroundСolor()
{
    return m_properties["backgroundСolor"].value<QColor>();
}

void Sample::toJson(QJsonObject &json) const
{
    m_properties.toJson(json);

    QJsonArray items;
    for (auto item : item_list) {
        QJsonObject item_json;
        item->toJson(item_json);
        items.append(item_json);
    }
    json["items"] = items;

    json["SwitchType"] = switch_event.type;
    json["SwitchTime"] = int(switch_event.time);
}

void Sample::fromJson(const QJsonObject &json)
{
    m_properties.fromJson(json);
/*
    if (json.contains("items") && json["items"].isArray()){
        QJsonArray items_json = json["items"].toArray();

        for(const QJsonValue &v : items_json) {
            QJsonObject item_json = v.toObject();

            if (!item_json.contains("ItemName")) return;

            QString itemType = item_json["ItemName"].toString();
            Item *item;
            if(itemType == "Spiral")
                item = CreateItem<SpiralItem>();
            else if(itemType == "Text")
                item = CreateItem<TextItem>();
            else
                item = nullptr;

            if(item != nullptr)
                item->fromJson(item_json);
        }
    }
*/
}
