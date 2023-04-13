#ifndef BUTTONITEM_H
#define BUTTONITEM_H

#include "../item.h"

class ButtonGraphicItem;
class ButtonItem : public Item
{
    Q_OBJECT
public:
    explicit ButtonItem(QObject *parent = nullptr);

    QGraphicsItem* graphicsItem() override;
    void setText(QString text, int size, QColor color = Qt::black);

    void setVisible(bool value);

private:
    ButtonGraphicItem* m_graphicButton = nullptr;

    friend class ButtonGraphicItem;
};

/////////////////////////////////////////////////////////////////////////////////////
/// \brief The ButtonGraphicItem class
/////////////////////////////////////////////////////////////////////////////////////

class ButtonGraphicItem : public QGraphicsItem
{
public:
    explicit ButtonGraphicItem(ButtonItem *parent);
    ~ButtonGraphicItem();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;


private:
    Properties *m_properties;
    ButtonItem *m_parent;

    QColor m_backgroundColor;
};

#endif // BUTTONITEM_H
