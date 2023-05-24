#include "lsl.h"
#include "player.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>

LSL::LSL()
{

}

bool LSL::open()
{
#ifdef USE_LSL_LIB
    lsl::stream_info info("NauroStim-Events", "Events", 1, lsl::IRREGULAR_RATE, lsl::cf_int32, "7643562-7455141");

    /* Facility*/
    lsl::xml_element facility = info.desc().append_child("facility");
    facility.append_child_value("lab", "Nurolab and Robotics")
        .append_child_value("department", "Neurology");

    //lsl::xml_element subject = info.desc().append_child("subject");

    /* Run Outlet */
    m_outlet.reset(new lsl::stream_outlet(info));
#endif
    m_isOpen = true;
    return true;
}

void LSL::close()
{
#ifdef USE_LSL_LIB
    m_outlet.reset();
#endif
    m_isOpen = false;
    stop();
}

void LSL::start()
{
    if(m_isOpen){
        m_events.clear();
        m_eventId = 0;
        m_isStart = true;
    }
}

void LSL::stop()
{
    m_isStart = false;
}

void LSL::saveEvents(QString file)
{
    if(m_events.size() > 0)
    {
        QJsonObject obj;
        QJsonArray events_json;
        for (const Event &event : m_events) {
            QJsonObject event_json;
            event.toJson(event_json);
            events_json.append(event_json);
        }
        obj["events"] = events_json;
        obj["version"] = "1.0.0";

        QFile saveFile(file);
        if (!saveFile.open(QIODevice::WriteOnly)) {
            qWarning("Couldn't open save file.");
            return;
        }
        saveFile.write(QJsonDocument(obj).toJson());
        saveFile.close();
    }
}

void LSL::setPlayer(Player *player)
{
    m_player = player;
}

void LSL::sendEvent(Event event)
{
    if(m_isStart)
    {
        event.id = m_eventId;
        event.sample = m_player->sampleNumber();
        event.time = local_clock();
        m_events.push_back(event);

#ifdef USE_LSL_LIB
        int32_t data[1] = {m_eventId};
        if(m_isOpen && m_outlet)
            m_outlet->push_sample(data);
#endif
        m_eventId++;
    }
}

double LSL::local_clock()
{
    #ifdef USE_LSL_LIB
        return lsl::local_clock();
    #endif

    int64_t clock_ns = std::chrono::nanoseconds(std::chrono::steady_clock::now().time_since_epoch()).count();
    const auto ns_per_s = 1000000000;
    const auto seconds_since_epoch = std::lldiv(clock_ns, ns_per_s);
    return seconds_since_epoch.quot + static_cast<double>(seconds_since_epoch.rem) / ns_per_s;
}

void Event::toJson(QJsonObject &json) const
{
    json["id"] = id;
    json["sample"] = sample;
    json["source"] = source;
    json["name"] = name;
    json["time"] = time;
    json["sourceid"] = sourceid;

    if(data.size()>0) {
        QJsonObject data_json;
        for (auto i = data.begin(); i != data.end(); ++i){
            data_json[i.key()] = i.value().toJsonValue();
        }
        json["data"] = data_json;
    }
}
