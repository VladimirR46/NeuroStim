#ifndef PLAYER_H
#define PLAYER_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QTimer>
#include "basetask.h"
#include "lsl.h"

class Player : public QGraphicsView
{
    Q_OBJECT
public:
    explicit Player();
    ~Player();

    void start(BaseTask *task, bool preview);
    void Stop();
    void playSample(Sample* sample);
    int sampleNumber();
    void saveTask();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void NextSample();
    void NextSampleSlot(Sample *sample);

signals:

private:
    void Loop();
    bool m_is_preview = false;

    QGraphicsScene *m_scene;
    QTimer* m_timer;

    BaseTask* m_task;
    Sample* m_current_sample;
    int m_sample_idx = 0;
    LSL m_lsl;
};

#endif // PLAYER_H
