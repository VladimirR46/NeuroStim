
#include "item.h"
#include <QSize>
#include "sample.h"

Item::Item(QObject *parent, QString name)
    : QObject(parent), m_name(name)
{
    m_properties.add(Property("pos",QPointF(0.0,0.0),"Позиция объекта",UIType::Edit));
    m_properties.add(Property("size",QSizeF(50.0,50.0),"Размер объекта",UIType::Edit));
}

void Item::start()
{
    m_lsl = getParentSample()->lsl();
    play();
}

QGraphicsItem *Item::graphicsItem()
{
    return nullptr;
}

void Item::play()
{
}

void Item::toJson(QJsonObject &json) const
{
    json["ItemName"] = m_name;
    json["ItemId"] = QString::number(m_id);
    m_properties.toJson(json);
}

void Item::fromJson(const QJsonObject &json)
{
    m_name = json["ItemName"].toString();
    m_id = json["ItemId"].toInt();
    m_properties.fromJson(json);
}

void Item::setId(int id)
{
    m_id = id;
}

void Item::setPosCenter(QSize screen)
{
    QSizeF size = m_properties["size"].value<QSizeF>();
    m_properties["pos"] = QPointF((screen.width()-size.width())/2, (screen.height()-size.height())/2);
}

Sample *Item::getParentSample()
{
    return static_cast<Sample*>(parent());
}

Properties *Item::properties()
{
    return &m_properties;
}

void Item::sendSwitchEvent()
{
    getParentSample()->switchEvent(this);
}

Item *Item::findItem(QString name)
{
    return getParentSample()->getItem(name);
}

QString Item::name()
{
    return m_name;
}


