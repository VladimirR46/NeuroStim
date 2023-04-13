#include "test1task.h"
#include "spiralitem.h"
#include "textitem.h"
#include "buttonitem.h"

Test1Task::Test1Task() : BaseTask("Моторные реакции")
{
    m_description = "В данном эксперименте испытуемому необходимо обводить спираль";

    if(!loadProperties())
    {
        m_properties.add(Property("TrialCount","4","Количество триалов",UIType::Edit));
        m_properties.add(Property("MonitorDiagonal", 23.8,"Диагональ монитора",UIType::Edit));
        m_properties.add(Property("InternRadius", 1.5 ,"Внутренний радиус",UIType::Edit));
        m_properties.add(Property("ExternRadius", 7.5 ,"Внешний радиус спирали",UIType::Edit));
        m_properties.add(Property("Spins", 3.0 ,"Количество витков",UIType::Edit));
        m_properties.add(Property("BeginPointColor", QColor(Qt::red) ,"Цвет точки начала",UIType::ColorDialog));
        m_properties.add(Property("Fone1", 4000 ,"Первый фон",UIType::Edit));
        m_properties.add(Property("Rest", 2000 ,"Отдых",UIType::Edit));
        m_properties.add(Property("isDebugDraw", false ,"Рисовать отладочную информацию",UIType::CheckBox));
    }
}

Test1Task::~Test1Task()
{
    qDebug()<<"~Test1Task";
}

bool Test1Task::Init(QSize screen)
{
    // Calc Pixel Size
    float D = m_properties.getFloat("MonitorDiagonal") * 2.54;
    float k = pow(screen.width()/float(screen.height()),2);
    float H = sqrt(D*D/(k+1));
    float pixelSize = H / screen.height(); // mm

    // init
    float spins = m_properties.getFloat("Spins");
    float ext_r = m_properties.getFloat("ExternRadius")/pixelSize;
    float int_r = m_properties.getFloat("InternRadius")/pixelSize;

    // init sequence
    std::vector<int> spiral_types(4*m_properties.getInt("TrialCount"));
    gen_sequence(spiral_types, 1, 4);
    std::shuffle(spiral_types.begin(), spiral_types.end(), g);

    // Make experiment
    TextItem *exp_beginText = CreateSample()->CreateItem<TextItem>();
    exp_beginText->setText("Запись фоновой активности\nОставляйте глаза открытыми",screen.height()/18);
    exp_beginText->setSize(screen);
    exp_beginText->setPosCenter(screen);
    exp_beginText->getParentSample()->setSwitchEvent(Events::TimeOut , m_properties.getInt("Fone1"));

    for(int i = 0; i < spiral_types.size(); i++)
    {
        // Rest 1
        CreateSample()->setSwitchEvent(Events::TimeOut, m_properties.getInt("Rest"));

        Sample* sample1 = CreateSample();
        sample1->setBackgroundСolor(Qt::white);

        // Spiral
        SpiralItem *spiral1 = sample1->CreateItem<SpiralItem>();
        spiral1->Init(spiral_types[i], spins, ext_r, int_r);
        spiral1->setPosCenter(screen);
        spiral1->setDrawProgress(m_properties.getBool("isDebugDraw"));

        // Button
        ButtonItem *button = sample1->CreateItem<ButtonItem>();
        button->setPos(screen.width()-200,screen.height()-100);
        button->setSize(150,50);
        button->setText("Продолжить",14);
        button->setVisible(false);

        sample1->setSwitchEvent(Events::ItemEvent, button);

        // Rest 2
        CreateSample()->setSwitchEvent(Events::TimeOut, m_properties.getInt("Rest"));
    }

    TextItem *exp_endText = CreateSample()->CreateItem<TextItem>();
    exp_endText->setText("Запись фоновой активности\nОставляйте глаза открытыми",screen.height()/18);
    exp_endText->setSize(screen);
    exp_endText->setPosCenter(screen);
    exp_endText->getParentSample()->setSwitchEvent(Events::TimeOut , 3000);

    return true;
}
