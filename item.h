#ifndef ITEM_H
#define ITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include "lsl.h"
#include "properties.h"

class Sample;
class Item : public QObject
{
    Q_OBJECT
public:
    explicit Item(QObject* parent, QString name);
    virtual ~Item(){qDebug()<<"~Item "+m_name;}

    void start();
    virtual QGraphicsItem* graphicsItem();
    virtual void play();

    void toJson(QJsonObject &json) const;
    void fromJson(const QJsonObject &json);

    void setId(int id);
    void setSize(QSizeF size) { m_properties["size"] = size; }
    void setSize(float w, float h) { setSize(QSize(w,h)); }
    void setPos(QPointF point) { m_properties["pos"] = point; }
    void setPos(float x, float y) { setPos(QPointF(x,y)); }
    void setRect(float x, float y, float w, float h) { setPos(x,y); setSize(w,h); }
    void setPosCenter(QSize screen);
    Sample *getParentSample();
    Properties* properties();
    void sendSwitchEvent();
    Item* findItem(QString name);
    QString name();

protected:
    QString m_name;
    int m_id;
    LSL *m_lsl;
    Properties m_properties;
};

#endif // ITEM_H
