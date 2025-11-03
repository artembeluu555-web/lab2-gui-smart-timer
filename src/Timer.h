#ifndef TIMER_H
#define TIMER_H

#pragma once
#include <QObject>
#include <QDateTime>
#include <QJsonObject>
#include <QUuid>

class Timer : public QObject {
    Q_OBJECT
public:
    explicit Timer(QObject* parent = nullptr);

    QString id() const { return m_id; }
    QString name() const { return m_name; }
    void setName(const QString& n) { m_name = n; }
    void setDuration(int seconds) { m_durationSec = seconds; }
    void setTarget(const QDateTime& t) { m_target = t; }

    void start();
    void stop();
    void tick();

    bool isRunning() const { return m_running; }
    qint64 remainingSeconds() const;

    QJsonObject toJson() const;
    static Timer* fromJson(const QJsonObject& obj, QObject* parent = nullptr);

    signals:
        void finished();

private:
    QString m_id;
    QString m_name;
    bool m_running{false};
    qint64 m_durationSec{0};
    QDateTime m_target;
    QDateTime m_startTime;
};

#endif
