#include "Timer.h"
#include <QJsonObject>
#include <QDateTime>
#include <QDebug>

Timer::Timer(QObject* parent)
    : QObject(parent),
      m_id(QUuid::createUuid().toString(QUuid::WithoutBraces)) {}

void Timer::start() {
    if (m_running)
        return;

    m_running = true;
    m_startTime = QDateTime::currentDateTime();
    m_target = m_startTime.addSecs(m_durationSec);
    qDebug() << "Timer started:" << m_name << "Duration:" << m_durationSec << "sec";
}

void Timer::stop() {
    if (!m_running)
        return;


    qint64 elapsed = m_startTime.secsTo(QDateTime::currentDateTime());
    m_durationSec = std::max<qint64>(0, m_durationSec - elapsed);
    m_running = false;
    qDebug() << "Timer stopped:" << m_name << "Remaining:" << m_durationSec << "sec";
}

qint64 Timer::remainingSeconds() const {
    if (!m_running)
        return m_durationSec;

    qint64 remaining = m_target.secsTo(QDateTime::currentDateTime()) * -1;
    return remaining > 0 ? remaining : 0;
}

void Timer::tick() {
    if (!m_running)
        return;

    qint64 remaining = remainingSeconds();
    if (remaining <= 0) {
        m_running = false;
        m_durationSec = 0;
        emit finished();
        qDebug() << "Timer finished and stopped:" << m_name;
    }
}

QJsonObject Timer::toJson() const {
    QJsonObject obj;
    obj["id"] = m_id;
    obj["name"] = m_name;
    obj["durationSec"] = static_cast<int>(remainingSeconds());
    obj["running"] = m_running;
    return obj;
}

Timer* Timer::fromJson(const QJsonObject& obj, QObject* parent) {
    auto* t = new Timer(parent);
    t->m_id = obj["id"].toString();
    t->m_name = obj["name"].toString();
    t->m_durationSec = obj["durationSec"].toInt();
    t->m_running = false;
    return t;
}
