#include "spiralitem.h"
#include "buttonitem.h"

SpiralItem::SpiralItem(QObject *parent)
    : Item(parent, "Spiral")
{
    m_properties.add(Property("InternRadius", 1.5 ,"Внутренний радиус",UIType::Edit));
    m_properties.add(Property("ExternRadius", 7.5 ,"Внешний радиус спирали",UIType::Edit));
    m_properties.add(Property("Spins", 3.0 ,"Количество витков",UIType::Edit));
    m_properties.add(Property("point_radius", 10 ,"Радиус кругов",UIType::Edit));
    m_properties.add(Property("type", 1 ,"Тип спирали",UIType::Edit));
    m_properties.add(Property("DrawProgress", false ,"Рисовать ошибку и заполнение", UIType::CheckBox));
}

SpiralItem::~SpiralItem()
{
    qDebug()<<"~SpiralItem";
}

void SpiralItem::Init(int type, float spins, float ext_r, float int_r)
{
    m_properties["ExternRadius"] = ext_r;
    m_properties["InternRadius"] = int_r;
    m_properties["Spins"] = spins;
    m_properties["type"] = type;

    m_properties["size"] = QSizeF(ext_r*2+50,ext_r*2+50);
}

void SpiralItem::setDrawProgress(bool value)
{
    m_properties["DrawProgress"] = value;
}

QGraphicsItem *SpiralItem::graphicsItem()
{
   m_graphic_item = new SpiralGraphicItem(this);
   m_graphic_item->setPos(m_properties.get<QPointF>("pos"));
   return m_graphic_item;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief SpiralGraphicItem
/// \return
/////////////////////////////////////////////////////////////////////////////////////////////////

SpiralGraphicItem::SpiralGraphicItem(SpiralItem *parent) : m_parent(parent)
{
    m_properties = parent->properties();

    isDrawProgress = m_properties->getBool("DrawProgress");
    spiral.type = m_properties->getInt("type");
    ext_radius = m_properties->getFloat("ExternRadius");
    point_radius = m_properties->getFloat("point_radius");

    if(spiral.type==1){ spiral.direction = -1; spiral.trace_type = 1;}
    if(spiral.type==2){ spiral.direction = 1; spiral.trace_type = 1;}
    if(spiral.type==3){ spiral.direction = -1; spiral.trace_type = 2;}
    if(spiral.type==4){ spiral.direction = 1; spiral.trace_type = 2;}

    spiral.step = 0.05;
    spiral.spins = m_properties->getFloat("Spins");
    spiral.ext_angle = spiral.spins*2*M_PI;
    spiral.k = m_properties->getFloat("ExternRadius")/spiral.ext_angle;
    spiral.int_angle = m_properties->getFloat("InternRadius")/spiral.k;

    QSizeF size = m_properties->get<QSizeF>("size");
    spiral.center = QPointF(size.width()/2, size.height()/2);
    spiral.begin = spiralEquation(spiral.direction,spiral.trace_type==1?spiral.int_angle:spiral.ext_angle,spiral.center,spiral.k);
    spiral.end  = spiralEquation(spiral.direction,spiral.trace_type==1?spiral.ext_angle:spiral.int_angle,spiral.center,spiral.k);
    TargetPoint = spiral.begin;
    lastProgressPoint = spiral.begin;
    lastPoint = spiral.begin;

    start_point = spiral.begin-spiral.center;
    start_point.setY(-start_point.y());

    start_angle = spiral.trace_type == 1 ? spiral.int_angle*spiral.direction : spiral.ext_angle*spiral.direction;
    min_angle = spiral.ext_angle - spiral.int_angle;
    last_current_point = start_point;
    last_data.phi = start_angle;
    relative_angle[0] = 0;
    cur_angle = 0;
    flag = 0;

    QImage newImage(m_properties->get<QSizeF>("size").toSize(), QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    image = newImage;
    is_run = true;
}

SpiralGraphicItem::~SpiralGraphicItem()
{
    qDebug()<<"~SpiralGraphicItem";
}

QRectF SpiralGraphicItem::boundingRect() const
{
    return QRect(0,0,ext_radius*2+50,ext_radius*2+50);
}

void SpiralGraphicItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawImage(boundingRect().x(),boundingRect().y(),image);
    painter->setRenderHint(QPainter::Antialiasing);
    drawSpiral(spiral, painter);
    drawCircle(spiral.begin, QColor(0x90ee90) ,point_radius, painter);
    drawCircle(spiral.end,QColor(0xff7f50),point_radius, painter);

    if(isDrawProgress)
        painter->drawLine(lastProgressPoint, lastPoint);
}

void SpiralGraphicItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (is_run && event->button() == Qt::LeftButton && CircleContain(event->pos(), TargetPoint, point_radius)) {
        lastPoint = event->pos();
        scribbling = true;

        Event lsl_event(m_parent->m_name, m_parent->m_id, "mousePress");
        lsl_event.data["x"] = lastPoint.x();
        lsl_event.data["y"] = lastPoint.y();
        m_parent->m_lsl->sendEvent(lsl_event);
    }
}

void SpiralGraphicItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && scribbling){
        drawLineTo(event->pos());

        if(CircleContain(event->pos(), spiral.end, point_radius))
        { 
            is_run = false;
            scribbling = false;

            Event lsl_event(m_parent->m_name, m_parent->m_id, "finish");
            m_parent->m_lsl->sendEvent(lsl_event);

            Item* item = m_parent->findItem("Button");
            if(item)
                static_cast<ButtonItem*>(item)->setVisible(true);

            //m_parent->sendSwitchEvent();

            return;
        }

        if(!CircleContain(event->pos(), TargetPoint, point_radius) && event->pos().x() > 0 && event->pos().y() > 0){
            PolarPoint point = calcAngle(event->pos());
            drawProgress(point.phi);

            Event lsl_event(m_parent->m_name, m_parent->m_id, "mouseMove");
            lsl_event.data["x"] = event->pos().x();
            lsl_event.data["y"] = event->pos().y();
            lsl_event.data["phi"] = point.phi;
            lsl_event.data["ro"] = point.ro;
            lsl_event.data["error"] = error;
            m_parent->m_lsl->sendEvent(lsl_event);
        }
    }
}

void SpiralGraphicItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && scribbling) {
        drawLineTo(event->pos());
        scribbling = false;
        TargetPoint = event->pos();

        Event lsl_event(m_parent->m_name, m_parent->m_id, "mouseRelease");
        lsl_event.data["x"] = event->pos().x();
        lsl_event.data["y"] = event->pos().y();
        m_parent->m_lsl->sendEvent(lsl_event);
    }
}

QPointF SpiralGraphicItem::spiralEquation(float dir, float step, QPointF center, float radius)
{
    QPointF point;
    point.setX(center.x() + (radius * step * sin(step)));
    point.setY(center.y() + (radius * step * cos(step))*dir);
    return point;
}

void SpiralGraphicItem::drawSpiral(Spiral &spiral, QPainter *painter)
{
    QPointF point1 = spiralEquation(spiral.direction,spiral.int_angle,spiral.center,spiral.k);
    for(float step = spiral.int_angle+spiral.step; step <= spiral.ext_angle; step+=spiral.step)
    {
        QPointF point2 = spiralEquation(spiral.direction,step,spiral.center,spiral.k);
        painter->setPen(QPen(Qt::black, 2, Qt::SolidLine));
        painter->drawLine(point1, point2);
        point1 = point2;
    }
}

void SpiralGraphicItem::drawLineTo(const QPointF &endPoint)
{
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(sPenColor, sPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);

    if(isDrawProgress){
        update();
    } else {
        int rad = (sPenWidth / 2) + 9;
        update(QRectF(lastPoint, endPoint).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));
    }
    lastPoint = endPoint;
}

void SpiralGraphicItem::drawCircle(QPointF point, QColor color, int radius, QPainter *painter)
{
    painter->setPen(QPen(Qt::black, 2, Qt::DashLine));
    painter->setBrush(QBrush(color, Qt::SolidPattern));
    painter->drawEllipse(QRect(point.x()-radius,point.y()-radius,radius*2,radius*2));
}

void SpiralGraphicItem::drawProgress(float angle)
{
    QPointF point = spiralEquation(spiral.direction,fabs(angle),spiral.center,spiral.k);
    if(isDrawProgress){
        QPainter painter(&image);
        painter.setPen(QPen(Qt::green, sPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawLine(lastProgressPoint, point);
    }
    lastProgressPoint = point;
    error = distance(lastProgressPoint, lastPoint);
}

PolarPoint SpiralGraphicItem::calcAngle(QPointF point)
{
    QPointF current_point = point-spiral.center;
    current_point.setY(-current_point.y());

    PolarPoint data = car2pol(current_point);

    relative_angle[1] = calc_relative_angle(current_point, last_current_point, relative_angle[0]);
    if(last_data.phi == start_angle)
    {
       if(((spiral.type==1 || spiral.type==4) ? -relative_angle[1] : relative_angle[1]) > 0) {
           data.phi = relative_angle[1] + start_angle;
           cur_angle = relative_angle[1];
       }
       else
           data.phi = start_angle;
    }
    else {
        float arg1 = (spiral.type==1 || spiral.type==4) ? -min_angle : cur_angle;
        float arg2 = (spiral.type==1 || spiral.type==4) ? cur_angle : min_angle;
        if(relative_angle[1] > arg1 && relative_angle[1] < arg2)
       {
          if(flag == 0)
             data.phi = calc_relative_angle(current_point, last_current_point , last_data.phi);
          else {
             data.phi = last_data.phi + (relative_angle[1] - cur_angle);
             flag = 0;
          }
          cur_angle = relative_angle[1];
       }
       else {
           data.phi = last_data.phi;
           flag = 1;
       }
    }
    last_data = data;
    last_current_point = current_point;
    relative_angle[0] = relative_angle[1];
    return data;
}

PolarPoint SpiralGraphicItem::car2pol(QPointF point)
{
    PolarPoint ret;
    ret.ro = sqrt(pow(point.x(),2) + pow(point.y(),2));

    if(ret.ro == 0) ret.phi = 0.0;
    else {
        ret.phi = acos(point.x()/ret.ro);
        if(point.y() < 0) ret.phi = 2*M_PI - ret.phi;
    }
    return ret;
}

float SpiralGraphicItem::calc_relative_angle(QPointF point1, QPointF point2, float base_angle)
{
    float dPhi = car2pol(point1).phi - car2pol(point2).phi;
    if(fabs(dPhi) < M_PI)
        return (base_angle + dPhi);
    else {
      if(dPhi > 0)
          return (base_angle + dPhi - 2*M_PI);
      else
          return (base_angle + dPhi + 2*M_PI);
    }
}

bool SpiralGraphicItem::CircleContain(QPointF point, QPointF &circle_point, float radius)
{
    float dist = distance(point, circle_point);
    return dist < radius;
}

float SpiralGraphicItem::distance(QPointF point1, QPointF point2)
{
    return sqrt(pow(point1.x()-point2.x(),2)+pow(point1.y()-point2.y(),2));
}
