#include "backgroundactivitytask.h"
#include "textitem.h"

BackgroundActivityTask::BackgroundActivityTask()  : BaseTask("Фоновая активность")
{
    m_description = "Эксперимент для записи фоновой активности с открытыми и закрытыми глазами";

    if(!loadProperties())
    {
        m_properties.add(Property("Background1",5000,"Фон 1 (глаза открыты)",UIType::Edit));
        m_properties.add(Property("Background2",5000,"Фон 2(глаза закрыты)",UIType::Edit));
        m_properties.add(Property("Background1Color", QColor(Qt::white) ,"Цвет фона",UIType::ColorDialog));
        m_properties.add(Property("TextColor", QColor(Qt::black) ,"Цвет текста",UIType::ColorDialog));
    }
}

BackgroundActivityTask::~BackgroundActivityTask()
{

}

bool BackgroundActivityTask::Init(QSize screen)
{
    Sample* background1 = CreateSample();
    background1->setBackgroundСolor(m_properties.getColor("Background1Color"));
    background1->setSwitchEvent(Events::TimeOut, m_properties.getInt("Background1"));

    TextItem *text1 = background1->CreateItem<TextItem>();
    text1->setText("Запись фоновой активности\nОставляйте глаза открытыми",screen.height()/18,m_properties.getColor("TextColor"));
    text1->setSize(screen);
    text1->setPosCenter(screen);

    Sample* background2 = CreateSample();
    background2->setBackgroundСolor(m_properties.getColor("Background1Color"));
    background2->setSwitchEvent(Events::TimeOut, m_properties.getInt("Background2"));

    TextItem *text2 = background2->CreateItem<TextItem>();
    text2->setText("Запись фоновой активности\nЗакройте глаза",screen.height()/18,m_properties.getColor("TextColor"));
    text2->setSize(screen);
    text2->setPosCenter(screen);

    return true;
}
