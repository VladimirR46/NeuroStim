#include "nasaitem.h"
#include <QMessageBox>
#include <vector>

NASAItem::NASAItem(QObject *parent)
    : Item{parent, "NASA"}
{
    m_timer = new QTimer(this);
    m_timer->setSingleShot(true);
    connect(m_timer, &QTimer::timeout, this, &NASAItem::TimeOut);
}

QGraphicsItem *NASAItem::graphicsItem()
{
    QSizeF size = m_properties.getSize("size");

    QGraphicsItemGroup *item = new QGraphicsItemGroup();
    item->setHandlesChildEvents(false);

    // Page 1
    page1 = new QGraphicsItemGroup();
    page1->setHandlesChildEvents(false);

    QGraphicsTextItem *htitle = new QGraphicsTextItem("С помощью шкалы опишите Ваши ощущения от прохождения задания");
    htitle->setTextWidth(size.width());
    htitle->setFont(QFont("Segoe UI",18));
    //title->setHtml("<div align='center'><span style='font-size:24px;'>"+txt+"</span></div>");
    htitle->setHtml("<div align='center'>" + htitle->toHtml() + "</div>");
    page1->addToGroup(htitle);

    QStringList left_label = {"НИЗКАЯ","НИЗКАЯ","НИЗКАЯ","ВЫСОКАЯ","МАЛО","НИЗКИЙ"};
    QStringList right_label = {"ВЫСОКАЯ","ВЫСОКАЯ","ВЫСОКАЯ","НИЗКАЯ","МНОГО","ВЫСОКИЙ"};

    categories = {"Умственная нагрузка",
                  "Физическая нагрузка",
                  "Нехватка времени",
                  "Эффективность",
                  "Усилия",
                  "Уровень недовольства"};

    desc_categories = {"Как много пришлось думать, решать и считать, чтобы пройти задание?",
                       "Объём и интенсивность физической активности, требуемой, чтобы завершить задания",
                       "Насколько сильно приходилось спешить, чтобы успеть завершить задание за отведенное время?",
                       "Насколько успешно удалось завершить задание?",
                       "Насколько много усилий пришлось приложить, чтобы сохранить эффективность выполнения заданий?",
                       "Насколько неуверенным и недовольным (или уверенным и довольным) вы себя чувствовали во время выполнения задания?"};

    float step = size.height()/7;
    for (int i = 0; i < 6; i++){
        ProgressButton *pbutton = new ProgressButton(800,50,20);
        pbutton->setPos(10,70+i*step);
        page1->addToGroup(pbutton);
        rating_buttons[i] = pbutton;

        QGraphicsTextItem *title_item = new QGraphicsTextItem(categories[i]);
        title_item->setTextWidth(800);
        title_item->setFont(QFont("Segoe UI",16, QFont::Bold));
        title_item->setHtml("<div align='center'>" + title_item->toHtml() + "</div>");
        title_item->setPos(10, 38+i*step);
        page1->addToGroup(title_item);

        QGraphicsTextItem *left_item = new QGraphicsTextItem(left_label[i]);
        left_item->setFont(QFont("Segoe UI",14));
        left_item->setPos(5, 40+i*step);
        page1->addToGroup(left_item);

        QGraphicsTextItem *right_item = new QGraphicsTextItem(right_label[i]);
        right_item->setTextWidth(100);
        right_item->setFont(QFont("Segoe UI",14));
        right_item->setHtml("<div align='right'>" + right_item->toHtml() + "</div>");
        right_item->setPos(15+700, 40+i*step);
        page1->addToGroup(right_item);

        QGraphicsTextItem *desc_item = new QGraphicsTextItem(desc_categories[i]);
        desc_item->setTextWidth(size.width() - 830);
        desc_item->setFont(QFont("Segoe UI",18));
        desc_item->setHtml("<div align='center'>" + desc_item->toHtml() + "</div>");
        desc_item->setPos(830, 45+i*step);
        page1->addToGroup(desc_item);
    }

    Button *button = new Button(100,30);
    button->setPos(size.width()-120,size.height()-35);
    button->setText("Продолжить");
    connect(button, &Button::onClick, this, &NASAItem::nextButton);

    page1->addToGroup(button);

    // Page 2
    page2 = new QGraphicsItemGroup();
    page2->setHandlesChildEvents(false);
    page2->setVisible(false);


    QGraphicsTextItem *p2header = new QGraphicsTextItem("Выберите характеристику, которая внесла наибольший вклад в нагрузку при выполнении задания");
    p2header->setTextWidth(size.width()-400);
    p2header->setFont(QFont("Segoe UI",24));
    p2header->setHtml("<div align='center'>" + p2header->toHtml() + "</div>");
    p2header->setPos(200,size.height()/4);
    page2->addToGroup(p2header);

    for(int i = 0; i < categories.size(); i++) {
        for(int j = i+1; j < categories.size(); j++) {
            std::vector<int> pair = {i,j};
            std::shuffle(pair.begin(), pair.end(), g);
            sequence.push_back(pair);
        }
    }
    std::shuffle(sequence.begin(), sequence.end(), g);

    float x_offset = 0.2;
    float between_offset = 0.1;
    float w = ((1 - x_offset*2 - between_offset)*size.width())/2;
    float h = size.height()/5;

    select_button0 = new Button(w, h);
    select_button0->setPos(x_offset*size.width(),size.height()/2.5);
    select_button0->setText("Текст 1");
    select_button0->setFontSize(20);
    page2->addToGroup(select_button0);
    connect(select_button0, &Button::onClick, this, &NASAItem::choiseButton);

    select_button1 = new Button(w, h);
    select_button1->setPos((x_offset+between_offset)*size.width()+w, size.height()/2.5);
    select_button1->setText("Текст 2");
    select_button1->setFontSize(20);
    page2->addToGroup(select_button1);
    connect(select_button1, &Button::onClick, this, &NASAItem::choiseButton);

    b0_desc = new QGraphicsTextItem("Выберите");
    b0_desc->setTextWidth(w);
    b0_desc->setFont(QFont("Segoe UI",18));
    b0_desc->setHtml("<div align='center'>" + b0_desc->toHtml() + "</div>");
    b0_desc->setPos(x_offset*size.width(),size.height()/2.5+h);
    page2->addToGroup(b0_desc);

    b1_desc = new QGraphicsTextItem("Выберите");
    b1_desc->setTextWidth(w);
    b1_desc->setFont(QFont("Segoe UI",18));
    b1_desc->setHtml("<div align='center'>" + b1_desc->toHtml() + "</div>");
    b1_desc->setPos((x_offset+between_offset)*size.width()+w,size.height()/2.5+h);
    page2->addToGroup(b1_desc);


    item->addToGroup(page1);
    item->addToGroup(page2);
    return item;
}

void NASAItem::nextButton(Button *button)
{
    for(int i = 0; i < 6; i++){
        if(rating_buttons[i]->getValue() == -1){
            QMessageBox msgBox;
            msgBox.setText("Заполните все шкалы");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
            return;
        }
    }

    page1->setVisible(false);
    m_timer->start(1000);
}

void NASAItem::TimeOut()
{
    if(iterator < sequence.size()) {
        page2->setVisible(true);
        select_button0->setVisible(true);
        select_button1->setVisible(true);
        b0_desc->setVisible(true);
        b1_desc->setVisible(true);

        select_button0->setText(categories[sequence[iterator][0]]);
        select_button0->setUserCode(sequence[iterator][0]);
        select_button1->setText(categories[sequence[iterator][1]]);
        select_button1->setUserCode(sequence[iterator][1]);

        b0_desc->setPlainText(desc_categories[sequence[iterator][0]]);
        b0_desc->setHtml("<div align='center'>" + b0_desc->toHtml() + "</div>");
        b1_desc->setPlainText(desc_categories[sequence[iterator][1]]);
        b1_desc->setHtml("<div align='center'>" + b1_desc->toHtml() + "</div>");

        iterator++;
    } else {

        QStringList titles = {"Mental","Physical","Temporal","Performance","Effort","Frustration"};

        Event lsl_event(m_name, m_id, "result");

        int data[6] = {0};
        for(int i = 0; i < titles.size(); i++) {
            data[i] = rating_buttons[i]->getValue()*5;
            lsl_event.data[titles[i]+"Rating"] = data[i];
        }

        int weighted_data[6] = {0};
        for(int i = 0; i < titles.size(); i++){
            weighted_data[i] = (data[i]*categories_counter[i])/5;
            lsl_event.data[titles[i]+"Weighted"] = weighted_data[i];
        }

        int rating_sum = std::accumulate(data, data + 6, 0);
        float mean_unweighted = (float) rating_sum / 6;
        lsl_event.data["MeanUnweighted"] = mean_unweighted;

        int weighted_data_sum = std::accumulate(weighted_data, weighted_data + 6, 0);
        float mean_weighted = (float) weighted_data_sum / 3;
        lsl_event.data["MeanWeighted"] = mean_weighted;

        m_lsl->sendEvent(lsl_event);

        sendSwitchEvent();
    }
}

void NASAItem::choiseButton(Button *button)
{
    select_button0->setVisible(false);
    select_button1->setVisible(false);
    b0_desc->setVisible(false);
    b1_desc->setVisible(false);
    m_timer->start(1000);

    categories_counter[button->getUserCode()]++;
}


/////////////////////////////////////////////////////////////////////
/// \brief ProgressButton::ProgressButton
////////////////////////////////////////////////////////////////////

ProgressButton::ProgressButton(float width, float height, int cell_count): m_width(width), m_height(height)
{
    setAcceptHoverEvents(true);

    cell_width = (width/cell_count)-1;
    for(int i = 0; i < cell_count; i++){
        Cell cell;
        cell.color = Qt::white;
        cell.border_color = Qt::gray;
        cell.rect = QRectF(cell_width*i+i, 0, cell_width, height);
        m_cells.append(cell);
    }
}

QRectF ProgressButton::boundingRect() const
{
    return QRectF(0,0,m_width,m_height);
}

void ProgressButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    for(auto cell : m_cells) {
        painter->setPen(QPen(cell.border_color, 1));
        painter->fillRect(cell.rect,cell.getColor());
        painter->drawRect(cell.rect);
    }

    painter->setPen(QPen(Qt::black, 2));
    painter->drawRect(boundingRect());
}

void ProgressButton::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    for (int i = 0; i < m_cells.size(); i++){
        m_cells[i].is_selected = true;
        if(m_cells[i].is_focused){
            value = i+1;
            break;
        }
    }

    for (int i = value; i < m_cells.size(); i++)
        m_cells[i].is_selected = false;
}

void ProgressButton::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    for(auto& cell : m_cells)
        cell.is_focused = cell.rect.contains(event->pos());

    update();
    //update(m_cells[index].rect);
}

void ProgressButton::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    for(auto& cell : m_cells)
        cell.is_focused = false;
    update();
}

int ProgressButton::getValue() const
{
    return value;
}

QColor ProgressButton::Cell::getColor()
{
    if(is_focused)
        return QColor(190,230,253);

    if(is_selected)
        return QColor(211,211,211);

    return Qt::white;
}

Button::Button(float width, float height): m_width(width), m_height(height)
{

}

QRectF Button::boundingRect() const
{
    return QRectF(0,0,m_width,m_height);
}

void Button::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->fillRect(boundingRect(),QColor(211,211,211));
    painter->drawRect(boundingRect());

    QFont font;
    font.setPointSize(m_font_size);
    painter->setFont(font);
    painter->drawText(boundingRect(),m_text, Qt::AlignCenter | Qt::AlignVCenter);
}

QString Button::text()
{
    return m_text;
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit onClick(this);
}

int Button::getUserCode() const
{
    return user_code;
}

void Button::setUserCode(int newUser_code)
{
    user_code = newUser_code;
}

void Button::setText(const QString &newText)
{
    m_text = newText;
}

void Button::setFontSize(int size)
{
    m_font_size = size;
}
