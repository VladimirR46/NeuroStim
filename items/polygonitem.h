#ifndef POLYGONITEM_H
#define POLYGONITEM_H

#include <../item.h>

enum class PolygonType
{
    Rect,
    Circle,
    Cross
};

struct PolygonDate
{
    QPen pen;

};

class PolygonItem : public Item
{
    Q_OBJECT
public:
    explicit PolygonItem(QObject *parent = nullptr);

    QGraphicsItem* graphicsItem() override;

    void setType(QString type);

private:
};

#endif // POLYGONITEM_H
