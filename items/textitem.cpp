#include "textitem.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>


TextItem::TextItem(QObject *parent)
    : Item(parent, "Text")
{
    m_properties.add(Property("text","","Текст",UIType::Edit));
    m_properties.add(Property("pointsize", 14 ,"Размер шрифта",UIType::Edit));
    m_properties.add(Property("textcolor", QColor(Qt::white) ,"Цвет текста", UIType::ColorDialog));
}

void TextItem::setText(QString text, int size, QColor color)
{
    m_properties["text"] = text;
    m_properties["pointsize"] = size;
    m_properties["textcolor"] = color;
}

QGraphicsItem *TextItem::graphicsItem()
{
    TextGraphicItem *text = new TextGraphicItem(this);
    text->setPos(m_properties.get<QPointF>("pos"));
    text->setFlags(QGraphicsItem::ItemIsMovable);
    return text;
}

//////////////////////////////////////////////////////////////////////////////////////
/// \brief TextGraphicItem::TextGraphicItem
/// \param parent
//////////////////////////////////////////////////////////////////////////////////////

TextGraphicItem::TextGraphicItem(TextItem *parent): m_parent(parent)
{
    m_properties = parent->properties();
}

TextGraphicItem::~TextGraphicItem()
{
    qDebug()<<"~TextGraphicItem";
}

QRectF TextGraphicItem::boundingRect() const
{
    return QRectF(QPointF(0,0), m_properties->get<QSizeF>("size"));
}

void TextGraphicItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QFont font;
    font.setPointSize(m_properties->getInt("pointsize"));
    painter->setFont(font);
    painter->setPen(m_properties->get<QColor>("textcolor"));
    painter->drawText(boundingRect(), m_properties->getString("text"), Qt::AlignCenter | Qt::AlignVCenter);
}
