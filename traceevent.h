#ifndef TRACEEVENT_H
#define TRACEEVENT_H

#include <QString>
#include <QDebug>

class TraceEvent
{
    TraceEvent();

public:
    QString threadName() const { return m_threadName; }
    pid_t pid() const { return m_pid; }
    int cpuNumber() const { return m_cpu; }
    timeval timestamp() const { return m_timestamp; }
    QString eventName() const { return m_eventName; }
    QString details() const { return m_details; }
    bool isValid() const;

    static TraceEvent fromString(const QByteArray &string);
private:
    QString m_threadName;
    pid_t m_pid;
    int m_cpu;
    timeval m_timestamp;
    QString m_eventName;
    QString m_details;
};

QDebug operator<<(QDebug dbg, const TraceEvent &event);

#endif // TRACEEVENT_H
