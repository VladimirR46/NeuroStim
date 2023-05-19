#include "mfiitem.h"

MFIItem::MFIItem(QObject *parent)
    : Item{parent, "MFI"}
{
    m_timer = new QTimer(this);
    m_timer->setSingleShot(true);
    connect(m_timer, &QTimer::timeout, this, &MFIItem::TimeOut);

    questions = {"Я чувствую себя бодро",
                             "Я не способен к большой физической нагрузке",
                             "Я чувствую себя очень энергично",
                             "Я хочу заниматься приятными вещами",
                             "Я чувствую себя уставшим",
                             "Мне кажется, за день я успеваю сделать многое",
                             "Я могу сосредоточиться на том, чем я занимаюсь",
                             "Я готов к большой физической нагрузке",
                             "Я боюсь необходимости заниматься делами",
                             "Мне кажется, что за день я успеваю очень мало",
                             "Я хорошо концентрируюсь",
                             "Я чувствую себя отдохнувшим",
                             "Я с трудом могу концентрироваться на чём-либо",
                             "Я считаю, что я в плохой физической форме",
                             "У меня много планов",
                             "Я быстро устаю",
                             "Я мало что делаю",
                             "Мне не хочется ничем заниматься",
                             "Я легко отвлекаюсь",
                             "Я считаю, что я в прекрасной физической форме"};
}

QGraphicsItem *MFIItem::graphicsItem()
{
    page = new QGraphicsItemGroup();
    page->setHandlesChildEvents(false);

    QSizeF size = m_properties.get<QSizeF>("size");
    float H = size.height()/5.5;
    float bWidth = size.width()/6;
    float bHeight = size.height()/5 - 40;

    for(int i = 0; i < 5; i++){
        Button *button1 = new Button(bWidth, bHeight);
        button1->setPos((size.width()/10)+(size.width()/5)*i-bWidth/2, H*3);
        button1->setText(QString::number(i+1));
        button1->setFontSize(28);
        button1->setUserCode(i+1);
        connect(button1, &Button::onClick, this, &MFIItem::choiseButton);
        page->addToGroup(button1);
    }

    header = new QGraphicsTextItem(questions[0]);
    header->setTextWidth(size.width()-200);
    header->setFont(QFont("Segoe UI",32));
    header->setHtml("<div align='center'>" + header->toHtml() + "</div>");
    header->setPos(100,H);
    page->addToGroup(header);

    QGraphicsTextItem *left = new QGraphicsTextItem("Да, это правда");
    left->setTextWidth(bWidth+50);
    left->setFont(QFont("Segoe UI", 28));
    left->setHtml("<div align='center'>" + left->toHtml() + "</div>");
    left->setPos(((size.width()/10)-bWidth/2)-25,H*3+bHeight);
    page->addToGroup(left);

    QGraphicsTextItem *right = new QGraphicsTextItem("Нет, это не правда");
    right->setTextWidth(bWidth+50);
    right->setFont(QFont("Segoe UI", 28));
    right->setHtml("<div align='center'>" + right->toHtml() + "</div>");
    right->setPos(((size.width()/10)+(size.width()/5)*4-bWidth/2)-25,H*3+bHeight);
    page->addToGroup(right);

    return page;
}

void MFIItem::TimeOut()
{
    iterator++;
    if(iterator < questions.size()) {
        page->setVisible(true);
        header->setPlainText(questions[iterator]);
        header->setHtml("<div align='center'>" + header->toHtml() + "</div>");
    } else {
        Event lsl_event(m_name, m_id, "result");
        lsl_event.data["PhysicalFatigue"] = (int) (6-answers[1])+answers[7]+(6-answers[13])+answers[19]; // !2 8 !14 20
        lsl_event.data["MentalFatigue"] = (int) answers[6]+answers[10]+(6-answers[12])+(6-answers[18]); // 7 11 !13 !19
        lsl_event.data["GeneralFatigue"] = (int) answers[0]+(6-answers[4])+answers[11]+(6-answers[15]); // 1 !5 12 !16
        lsl_event.data["ReducedActivity"] = (int) answers[2]+answers[5]+(6-answers[9])+(6-answers[16]); // 3 6 !10 !17
        lsl_event.data["ReducedMotivation"] = (int) answers[3]+(6-answers[8])+answers[14]+(6-answers[17]); // 4 !9 15 !18
        m_lsl->sendEvent(lsl_event);

        sendSwitchEvent();
    }
}

void MFIItem::choiseButton(Button *button)
{
    page->setVisible(false);
    m_timer->start(500);
    answers[iterator] = button->getUserCode();
}
