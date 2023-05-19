#ifndef MFIITEM_H
#define MFIITEM_H

#include "../item.h"
#include "nasaitem.h"
#include <QTimer>

class MFIItem : public Item
{
    Q_OBJECT
public:
    explicit MFIItem(QObject *parent = nullptr);

    QGraphicsItem* graphicsItem() override;

private slots:
    void TimeOut();
    void choiseButton(Button *button);

private:
    QTimer *m_timer;
    QStringList questions;
    int answers[20];
    QGraphicsItemGroup *page;
    QGraphicsTextItem *header;
    int iterator = 0;

};

#endif // MFIITEM_H
