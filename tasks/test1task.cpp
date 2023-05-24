#include "test1task.h"
#include "spiralitem.h"
#include "textitem.h"
#include "buttonitem.h"
#include "mfiitem.h"
#include "nasaitem.h"

Test1Task::Test1Task() : BaseTask("Моторные реакции")
{
    m_description = "В данном эксперименте испытуемому необходимо обводить спираль";

    if(!loadProperties())
    {
        m_properties.add(Property("TrialCount","3","Количество триалов",UIType::Edit));
        m_properties.add(Property("MonitorDiagonal", 11,"Диагональ монитора",UIType::Edit));
        m_properties.add(Property("InternRadius", 1.5 ,"Внутренний радиус",UIType::Edit));
        m_properties.add(Property("ExternRadius", 7 ,"Внешний радиус спирали",UIType::Edit));
        m_properties.add(Property("Spins", 5.0 ,"Количество витков",UIType::Edit));
        m_properties.add(Property("LineWidth", 7 ,"Толщина линии",UIType::Edit));
        m_properties.add(Property("BeginPointColor", QColor(0x90ee90) ,"Цвет точки начала",UIType::ColorDialog));
        m_properties.add(Property("EndPointColor", QColor(0xff7f50) ,"Цвет точки конца",UIType::ColorDialog));
        m_properties.add(Property("Fone1", 100 ,"Первый фон",UIType::Edit));
        m_properties.add(Property("Rest", 2000 ,"Отдых",UIType::Edit));
        m_properties.add(Property("isDebugDraw", false ,"Рисовать отладочную информацию",UIType::CheckBox));
        m_properties.add(Property("isTimeLimit", false ,"Ограничение по времени",UIType::CheckBox));
        m_properties.add(Property("isNonLimit", true ,"Без ограничений",UIType::CheckBox));
        m_properties.add(Property("TimeLimitValue", 7000 ,"Время ограничения",UIType::Edit));
        m_properties.add(Property("isNASA", true ,"Тест NASA",UIType::CheckBox));
        m_properties.add(Property("isMFI", true ,"Тест MFI-20",UIType::CheckBox));
    }
}

Test1Task::~Test1Task()
{
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

    struct SpiralType {
        int type;
        int time_limit;
    };

    bool isLimit = m_properties.getBool("isTimeLimit");
    bool isNoneLimit = m_properties.getBool("isNonLimit");

    std::vector<SpiralType> spirals;
    for (int i = 0; i < m_properties.getInt("TrialCount"); i++)
        for(int j = 0; j < 2; j++)
            for (int k = 0; k < 4; k++) {
                SpiralType spiral;
                spiral.type = k+1;
                spiral.time_limit = (isLimit && isNoneLimit) ? j : isLimit ? 1 : 0 ;
                spirals.push_back(spiral);
            }
    std::shuffle(spirals.begin(), spirals.end(), g);

    // MFI
    if(m_properties.getBool("isMFI")){
        MFIItem *mfi1 = CreateSample()->CreateItem<MFIItem>();
        mfi1->setSize(screen);
        mfi1->getParentSample()->skipInPreview(true);
        mfi1->getParentSample()->setSwitchEvent(Events::ItemEvent, mfi1);
    }

    // Make experiment
    TextItem *exp_beginText = CreateSample()->CreateItem<TextItem>();
    exp_beginText->setText("Запись фоновой активности\nОставляйте глаза открытыми",screen.height()/18);
    exp_beginText->setSize(screen);
    exp_beginText->setPosCenter(screen);
    exp_beginText->getParentSample()->setSwitchEvent(Events::TimeOut , m_properties.getInt("Fone1"));
    exp_beginText->getParentSample()->skipInPreview(true);

    for(int i = 0; i < spirals.size(); i++)
    {
        // Rest 1
        if (m_properties.getBool("isTimeLimit") && m_properties.getBool("isNonLimit")){
            TextItem *time_limit_text = CreateSample()->CreateItem<TextItem>();
            time_limit_text->setText(spirals[i].time_limit==0? "Нет ограничений по времени" : "Ограничение по времени",screen.height()/18);
            time_limit_text->setSize(screen);
            time_limit_text->setPosCenter(screen);
            time_limit_text->getParentSample()->setSwitchEvent(Events::TimeOut , m_properties.getInt("Rest"));
        }

        Sample* sample1 = CreateSample();
        sample1->setBackgroundСolor(Qt::white);

        // Spiral
        SpiralItem *spiral1 = sample1->CreateItem<SpiralItem>();
        spiral1->Init(spirals[i].type, spins, ext_r, int_r, spirals[i].time_limit);
        spiral1->setPosCenter(screen);
        spiral1->setDrawProgress(m_properties.getBool("isDebugDraw"));
        spiral1->properties()->setPropertyValue("LineWidth", m_properties["LineWidth"]);
        spiral1->properties()->setPropertyValue("TimeLimitValue", m_properties["TimeLimitValue"]);
        spiral1->properties()->setPropertyValue("BeginPointColor", m_properties["BeginPointColor"]);
        spiral1->properties()->setPropertyValue("EndPointColor", m_properties["EndPointColor"]);

        // Button
        ButtonItem *button = sample1->CreateItem<ButtonItem>();
        button->setPos(screen.width()-200,screen.height()-100);
        button->setSize(150,50);
        button->setText("Продолжить",14);
        button->setVisible(false);

        sample1->setSwitchEvent(Events::ItemEvent, button);
    }

    TextItem *exp_endText = CreateSample()->CreateItem<TextItem>();
    exp_endText->setText("Запись фоновой активности\nОставляйте глаза открытыми",screen.height()/18);
    exp_endText->setSize(screen);
    exp_endText->setPosCenter(screen);
    exp_endText->getParentSample()->setSwitchEvent(Events::TimeOut , 3000);
    exp_endText->getParentSample()->skipInPreview(true);

    // NASA
    if(m_properties.getBool("isNASA")){
        NASAItem *nasa = CreateSample()->CreateItem<NASAItem>();
        nasa->setSize(screen);
        nasa->getParentSample()->setSwitchEvent(Events::ItemEvent, nasa);
        nasa->getParentSample()->skipInPreview(true);
    }

    // MFI
    if(m_properties.getBool("isMFI")){
        MFIItem *mfi2 = CreateSample()->CreateItem<MFIItem>();
        mfi2->setSize(screen);
        mfi2->getParentSample()->skipInPreview(true);
        mfi2->getParentSample()->setSwitchEvent(Events::ItemEvent, mfi2);
    }

    return true;
}
