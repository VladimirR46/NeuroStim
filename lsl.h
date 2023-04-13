#ifndef LSL_H
#define LSL_H

#include <chrono>
#include <memory>
#include <list>
#include <QString>
#include <QMap>
#include <QVariant>
#include <chrono>

#ifdef USE_LSL_LIB
    #include <lsl_cpp.h>
#endif


struct Event
{
    Event(){}
    Event(QString source, int sourceid, QString name):source(source),
        name(name),sourceid(sourceid) {}

    int id;
    int sample;
    QString source;
    QString name;
    double time;
    int sourceid;
    QMap<QString, QVariant> data;

    void toJson(QJsonObject &json) const;
};

class Player;
class LSL
{
public:
    LSL();

    bool open();
    void close();
    void start();
    void stop();
    void saveEvents(QString file);
    void setPlayer(Player *player);
    void sendEvent(Event event);

    double local_clock();

private:
    Player *m_player;

    #ifdef USE_LSL_LIB
        std::shared_ptr<lsl::stream_outlet> m_outlet;
    #endif

    bool m_isOpen = false;
    bool m_isStart = false;
    int m_eventId = 0;

    std::list<Event> m_events;
};

#endif // LSL_H
