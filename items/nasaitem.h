#ifndef NASAITEM_H
#define NASAITEM_H

#include "../item.h"
#include <QList>
#include <QTimer>
#include <random>

class ProgressButton;
class Button;

class NASAItem : public Item
{
    Q_OBJECT
public:
    explicit NASAItem(QObject *parent = nullptr);

    QGraphicsItem* graphicsItem() override;

private slots:
    void nextButton(Button *button);
    void TimeOut();
    void choiseButton(Button *button);

private:
    QTimer *m_timer;
    QGraphicsItemGroup *page1;
    QGraphicsItemGroup *page2;
    int m_state = 0;

    Button *select_button0;
    Button *select_button1;
    QGraphicsTextItem *b0_desc;
    QGraphicsTextItem *b1_desc;
    ProgressButton* rating_buttons[6];

    std::random_device rd;
    std::mt19937 g{rd()};

    QStringList categories;
    QStringList desc_categories;
    std::vector<std::vector<int>> sequence;
    int iterator = 0;
    int categories_counter[6] = {0};
};

/////////////////////////////////////////////////////////////////////
/// \brief The Button class
/////////////////////////////////////////////////////////////////////

class Button : public QGraphicsObject
{
    Q_OBJECT
public:
    Button(float width = 100, float height = 40);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QString text();
    void setText(const QString &newText);
    void setFontSize(int size);

    int getUserCode() const;
    void setUserCode(int newUser_code);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    void onClick(Button *button);

private:
    QString m_text;
    float m_width;
    float m_height;
    int m_font_size = 12;
    int user_code = 0;
};

/////////////////////////////////////////////////////////////////////////////////
/// \brief The ProgressButton class
/////////////////////////////////////////////////////////////////////////////////

class ProgressButton : public QGraphicsItem
{
    struct Cell {
        QColor color;
        QColor border_color;
        QRectF rect;
        bool is_selected = false;
        bool is_focused = false;

        QColor getColor();
    };

public:
    ProgressButton(float width = 100, float height = 40, int cell_count = 20);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    int getValue() const;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event)  override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    QList<Cell> m_cells;
    float cell_width;
    float m_width;
    float m_height;
    int value = -1;
};

#endif // NASAITEM_H
