#include "polygonitem.h"

PolygonItem::PolygonItem(QObject *parent)
    : Item(parent, "Polygon")
{
    m_properties.add(Property("type","rect","Тип объекта",UIType::Edit));
}

QGraphicsItem *PolygonItem::graphicsItem()
{
    QGraphicsItem* item = nullptr;
    QString type = m_properties.getString("type");
    QSizeF size = m_properties.getSize("size");
    QPointF pos = m_properties.get<QPointF>("pos");

    if(type == "circle") {
        QGraphicsEllipseItem* circle = new QGraphicsEllipseItem(QRectF(0,0,size.width(),size.height()));
        circle->setBrush(QBrush(Qt::black));
        item = circle;
        item->setPos(pos.x()-size.width()/2, pos.y()-size.height()/2);
    }

    Event lsl_event(m_name+"."+type, m_id, "show");
    lsl_event.data["x"] = pos.x();
    lsl_event.data["y"] = pos.y();
    lsl_event.data["w"] = size.width();
    lsl_event.data["h"] = size.height();
    m_lsl->sendEvent(lsl_event);

    return item;
}

void PolygonItem::setType(QString type)
{
    m_properties["type"] = type;
}
