#include "pointfindertask.h"
#include <QPointF>
#include <QRandomGenerator>
#include "polygonitem.h"
#include "textitem.h"

PointFinderTask::PointFinderTask() : BaseTask("Поиск точек")
{
    m_description = "Эксперимент в котором испытуемый ищет случайные точки на экране";

    if(!loadProperties())
    {
        m_properties.add(Property("TrialCount", 4 ,"Количество триалов",UIType::Edit));
        m_properties.add(Property("TrialDuration",5000,"Длительность точки", UIType::Edit));
        m_properties.add(Property("PointSize", 20, "Размер точки", UIType::Edit));
    }
}

PointFinderTask::~PointFinderTask()
{

}

bool PointFinderTask::Init(QSize screen)
{
    QList<QPointF> points = generateRandomPoints(screen);


    TextItem *exp_beginText = CreateSample()->CreateItem<TextItem>();
    exp_beginText->setText("Приготовьтесь",screen.height()/18);
    exp_beginText->setSize(screen);
    exp_beginText->setPosCenter(screen);
    exp_beginText->getParentSample()->setSwitchEvent(Events::TimeOut , 3000);


    for(int i = 0; i < points.size(); i++) {
        PolygonItem* item = CreateSample()->CreateItem<PolygonItem>();
        item->setType("circle");
        item->setSize(QSizeF(m_properties.getFloat("PointSize"),m_properties.getFloat("PointSize")));
        item->setPos(points[i]);
        item->getParentSample()->setSwitchEvent(Events::TimeOut, m_properties.getInt("TrialDuration"));
        item->getParentSample()->setLslSending(false);
    }

    return true;
}

QList<QPointF> PointFinderTask::generateRandomPoints(QSize screen)
{
    QList<QPointF> points;
    QRandomGenerator generator;
    generator.seed(QDateTime::currentMSecsSinceEpoch()); // Инициализация генератора случайных чисел

    int offset = m_properties.getInt("PointSize")/2;
    for (int i = 0; i < m_properties.getInt("TrialCount"); ++i) {
       qreal x = generator.bounded(offset, screen.width()-offset); // Генерация случайной координаты x
       qreal y = generator.bounded(offset, screen.height()-offset); // Генерация случайной координаты y
       points.append(QPointF(x, y)); // Добавление точки в список
    }

    return points;
}
