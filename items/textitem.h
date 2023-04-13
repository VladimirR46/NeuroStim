#ifndef TEXTITEM_H
#define TEXTITEM_H

#include <../item.h>
#include <../properties.h>

class TextItem : public Item
{
    Q_OBJECT
public:
    explicit TextItem(QObject *parent = nullptr);

    void setText(QString text, int size, QColor color = Qt::black);
    QGraphicsItem* graphicsItem() override;

private:
    friend class TextGraphicItem;
};

///////////////////////////////////////////////////////////////////////////
/// \brief The TextGraphicItem class
///////////////////////////////////////////////////////////////////////////

class TextGraphicItem : public QGraphicsItem
{
public:
    explicit TextGraphicItem(TextItem *parent);
    ~TextGraphicItem();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    Properties *m_properties;
    TextItem *m_parent;
};

#endif // TEXTITEM_H
