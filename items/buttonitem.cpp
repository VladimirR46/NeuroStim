#include "buttonitem.h"

ButtonItem::ButtonItem(QObject *parent)
    : Item{parent, "Button"}
{
    m_properties.add(Property("text","Кнопка","Текст",UIType::Edit));
    m_properties.add(Property("pointsize", 14 ,"Размер шрифта",UIType::Edit));
    m_properties.add(Property("textcolor", QColor(Qt::black) ,"Цвет текста", UIType::ColorDialog));
    m_properties.add(Property("visible", true ,"Видимость объекта", UIType::CheckBox));
}

QGraphicsItem *ButtonItem::graphicsItem()
{
    m_graphicButton = new ButtonGraphicItem(this);
    m_graphicButton->setPos(m_properties.get<QPointF>("pos"));
    m_graphicButton->setVisible(m_properties["visible"].toBool());
    return m_graphicButton;
}

void ButtonItem::setText(QString text, int size, QColor color)
{
    m_properties["text"] = text;
    m_properties["pointsize"] = size;
    m_properties["textcolor"] = color;
}

void ButtonItem::setVisible(bool value)
{
    m_properties["visible"] = value;
    if(m_graphicButton)
        m_graphicButton->setVisible(value);
}

////////////////////////////////////////////////////////////////////////
/// \brief ButtonGraphicItem::ButtonGraphicItem
/// \param parent
///////////////////////////////////////////////////////////////////////

ButtonGraphicItem::ButtonGraphicItem(ButtonItem *parent): m_parent(parent)
{
    m_properties = parent->properties();
    m_backgroundColor = 0xE6E6E6;
    setAcceptHoverEvents(true);
}

ButtonGraphicItem::~ButtonGraphicItem()
{

}

QRectF ButtonGraphicItem::boundingRect() const
{
    return QRectF(QPointF(0,0), m_properties->get<QSizeF>("size"));
}

void ButtonGraphicItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QPen(Qt::black,4));
    painter->fillRect(boundingRect(),m_backgroundColor);
    painter->drawRect(boundingRect());
    // text
    QFont font;
    font.setPointSize(m_properties->getInt("pointsize"));
    painter->setFont(font);
    painter->setPen(m_properties->get<QColor>("textcolor"));
    painter->drawText(boundingRect(), m_properties->getString("text"), Qt::AlignCenter | Qt::AlignVCenter);
}

void ButtonGraphicItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Event lsl_event(m_parent->m_name, m_parent->m_id, "mousePress");
    m_parent->m_lsl->sendEvent(lsl_event);

    m_parent->sendSwitchEvent();
}

void ButtonGraphicItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    m_backgroundColor = 0xF5F5F5;
    update();
}

void ButtonGraphicItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_backgroundColor = 0xE6E6E6;
    update();
}
