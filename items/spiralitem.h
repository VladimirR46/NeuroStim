#ifndef SPIRAL_H
#define SPIRAL_H

#include <../item.h>


class SpiralGraphicItem;
class SpiralItem : public Item
{
    Q_OBJECT
public:
    explicit SpiralItem(QObject* parent);
    ~SpiralItem();

    void Init(int type, float spins, float ext_r, float int_r);
    void setDrawProgress(bool value);
    QGraphicsItem* graphicsItem() override;

private:
    SpiralGraphicItem *m_graphic_item;

    friend class SpiralGraphicItem;
};

////////////////////////////////////////////////////////////////////////////////
/// \brief The SpiralGraphicItem class
////////////////////////////////////////////////////////////////////////////////

struct Spiral
{
    int type;
    int trace_type;
    int spins;
    int direction;
    float step;
    float k;
    float ext_angle;
    float int_angle;
    QPointF center;
    QPointF begin;
    QPointF end;
};

struct PolarPoint {
    float phi;
    float ro;
};

class SpiralGraphicItem : public QGraphicsItem
{
public:
    explicit SpiralGraphicItem(SpiralItem *parent);
    ~SpiralGraphicItem();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QPointF spiralEquation(float dir, float step, QPointF center, float radius);
    void drawSpiral(Spiral &spiral, QPainter *painter);
    void drawLineTo(const QPointF &endPoint);
    void drawCircle(QPointF point, QColor color, int radius, QPainter *painter);
    void drawProgress(float angle);
    PolarPoint calcAngle(QPointF point);
    PolarPoint car2pol(QPointF point);
    float calc_relative_angle(QPointF point1, QPointF point2, float base_angle);
    bool CircleContain(QPointF point, QPointF &circle_point, float radius);
    float distance(QPointF point1, QPointF point2);

    QImage image;
    int sPenWidth = 14;
    QColor sPenColor = 0x87ceeb;//Qt::blue;
    QPointF lastPoint;
    bool scribbling = false;
    Spiral spiral;
    QPointF TargetPoint;
    QPointF lastProgressPoint;
    bool is_run = false;

    //calc
    QPointF start_point;
    float start_angle;
    float relative_angle[2] = {0.0,0.0};
    float cur_angle;
    int flag;
    PolarPoint last_data;
    QPointF last_current_point;
    float min_angle;
    float ext_radius;
    float point_radius;
    bool isDrawProgress = false;
    float error = 0.0;

    //cfg
    Properties *m_properties;
    SpiralItem *m_parent;
};

#endif // SPIRAL_H
