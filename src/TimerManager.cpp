#include "TimerManager.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>

TimerManager::TimerManager(QObject* parent)
    : QObject(parent) {
    connect(&m_tick, &QTimer::timeout, this, &TimerManager::tick);
    m_tick.setInterval(1000);
    m_tick.start();
}


void TimerManager::addTimer(std::shared_ptr<Timer> timer) {
    m_timers.append(timer);
    emit timerUpdated();
}


void TimerManager::removeTimer(const QString& id) {
    for (int i = 0; i < m_timers.size(); ++i) {
        if (m_timers[i]->id() == id) {
            m_timers.removeAt(i);
            emit timerUpdated();
            break;
        }
    }
}


void TimerManager::startTimer(const QString& id) {
    qDebug() << "startTimer called for id:" << id;
    for (auto& t : m_timers) {
        if (t->id() == id) {
            t->start();
            qDebug() << "Timer started successfully:" << id;


            if (!m_tick.isActive()) {
                m_tick.start(1000);
                qDebug() << "Main tick started";
            }
        }
    }
    emit timerUpdated();
}


void TimerManager::stopTimer(const QString& id) {
    for (auto& t : m_timers) {
        if (t->id() == id) {
            t->stop();
            qDebug() << "Timer stopped:" << id;
        }
    }


    bool anyRunning = false;
    for (auto& t : m_timers)
        if (t->isRunning()) anyRunning = true;

    if (!anyRunning && m_tick.isActive()) {
        m_tick.stop();
        qDebug() << "All timers stopped — main tick paused";
    }

    emit timerUpdated();
}

void TimerManager::tick() {
    bool anyRunning = false;

    for (auto& t : m_timers) {
        if (t->isRunning()) {
            anyRunning = true;
            t->tick();

            qDebug() << "Tick for timer:" << t->id()
                     << "remaining:" << t->remainingSeconds();

            if (t->remainingSeconds() <= 0) {
                t->stop();
                emit timerFinished(t->id());
                qDebug() << "Timer finished:" << t->id();
            }
        }
    }

    emit timerUpdated();


    if (!anyRunning && m_tick.isActive()) {
        m_tick.stop();
        qDebug() << "No active timers, stopping tick.";
    }
}


void TimerManager::saveToFile(const QString& path) {
    QJsonArray arr;
    for (auto& t : m_timers)
        arr.append(t->toJson());

    QFile f(path);
    if (f.open(QIODevice::WriteOnly)) {
        f.write(QJsonDocument(arr).toJson());
        f.close();
        qDebug() << "Timers saved to file:" << path;
    }
}


void TimerManager::loadFromFile(const QString& path) {
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file for reading:" << path;
        return;
    }

    QByteArray data = f.readAll();
    f.close();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (!doc.isArray()) return;

    m_timers.clear();
    for (auto val : doc.array()) {
        m_timers.append(std::shared_ptr<Timer>(Timer::fromJson(val.toObject(), this)));
    }

    qDebug() << "Timers loaded from file:" << path;
    emit timerUpdated();
}
