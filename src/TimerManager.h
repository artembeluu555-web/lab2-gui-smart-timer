#ifndef TIMERMANAGER_H
#define TIMERMANAGER_H

#pragma once
#include <QObject>
#include <QTimer>
#include <QVector>
#include <memory>
#include "Timer.h"

class TimerManager : public QObject {
    Q_OBJECT
public:
    explicit TimerManager(QObject* parent = nullptr);
    void addTimer(std::shared_ptr<Timer> timer);
    void removeTimer(const QString& id);
    QVector<std::shared_ptr<Timer>> timers() const { return m_timers; }
    void startTimer(const QString& id);
    void stopTimer(const QString& id);
    void saveToFile(const QString& path);
    void loadFromFile(const QString& path);

    signals:
        void timerUpdated();
    void timerFinished(const QString& id);

private slots:
    void tick();

private:
    QVector<std::shared_ptr<Timer>> m_timers;
    QTimer m_tick;
};
#endif