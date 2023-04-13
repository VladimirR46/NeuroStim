#include "player.h"
#include <QPointer>
#include <QStandardPaths>
#include <QDir>
#include <QOperatingSystemVersion>
#include <QPushButton>

Player::Player()
{
    m_lsl.setPlayer(this);
    //setAttribute(Qt::WA_StaticContents);
    setFrameStyle(0);
    m_scene = new QGraphicsScene();
    setScene(m_scene);

    m_timer = new QTimer(this);
    m_timer->setTimerType(Qt::PreciseTimer);
    m_timer->setSingleShot(true);
    connect(m_timer, &QTimer::timeout, this, &Player::NextSample);

    QPushButton *closeButton = new QPushButton("X", this);
    closeButton->setGeometry(0, 0, 30, 30);
    closeButton->setStyleSheet("border: none;");
    connect(closeButton, &QPushButton::clicked, this, [=](){ Stop(); });

    m_lsl.open();
}

Player::~Player()
{
    m_lsl.close();
}

void Player::start(BaseTask* task)
{
    if(!task) return;

    switch (QOperatingSystemVersion::currentType()) {
    case QOperatingSystemVersion::Windows:
        showFullScreen();
        break;
    case QOperatingSystemVersion::Android:
        show();
        break;
    default:
        return;
        break;
    }

    m_lsl.start();

    m_task = task;
    m_sample_idx = 0;
    if(m_task->start(size()))
        Loop();
    else
        Stop();
}

void Player::Stop()
{
    m_timer->stop();
    m_lsl.stop();
    m_scene->clear();
    saveTask();
    m_task->close();

    if(QOperatingSystemVersion::currentType() == QOperatingSystemVersion::Android)
        hide();
    else
        close();
}

void Player::playSample(Sample *sample)
{  
    m_scene->setBackgroundBrush(sample->backgroundСolor());

    for(int idx = 0; idx < sample->itemCount(); idx++)
    {
        Item* item = sample->getItem(idx);
        item->start();
        m_scene->addItem(item->graphicsItem());
        //m_scene->addRect(item->graphicsItem()->sceneBoundingRect(),QPen(Qt::red));
    }

    switch(sample->getSwitchEvent()->type)
    {
        case Events::TimeOut:
            m_timer->start(sample->getSwitchEvent()->time);
            break;
        case Events::ItemEvent:   
            connect(sample, &Sample::SwitchSampleSignal, this, &Player::NextSampleSlot, Qt::QueuedConnection);
            break;
    }
}

int Player::sampleNumber()
{
    return m_sample_idx;
}

void Player::saveTask()
{
    QString DocumentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString dtime = QDateTime::currentDateTime().toString("dd.MM.yyyy hh.mm.ss");

    Subject subject = m_task->subject();
    QString subjInfo = subject.Name+" "+subject.LastName;
    QString ProtocolDir = DocumentsPath+"/NeuroStim/Experiments/"+m_task->name()+"/"+subjInfo+dtime+"/";

    QDir dir(ProtocolDir);
    if (!dir.mkpath(ProtocolDir))
        return;

    m_task->save(ProtocolDir+"Task.json");
    m_lsl.saveEvents(ProtocolDir+"LslEvents.json");
}

void Player::resizeEvent(QResizeEvent *event)
{
    if(scene())
        scene()->setSceneRect(QRectF(QPointF(0, 0),event->size()));
    QGraphicsView::resizeEvent(event);
}

void Player::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
        Stop();
}

void Player::Loop()
{
    m_scene->clear();
    if(m_sample_idx < m_task->sampleCount()) {
        m_current_sample = m_task->getSample(m_sample_idx);
        m_current_sample->start(&m_lsl);
        playSample(m_current_sample);
    }
    else
        Stop();
}

void Player::NextSample()
{
    m_current_sample->stop();
    m_sample_idx++;
    Loop();
}

void Player::NextSampleSlot(Sample *sample)
{
    sample->disconnect(); // disconnect SwitchSampleSignal
    NextSample();
}
