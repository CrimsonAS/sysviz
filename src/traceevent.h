#ifndef TRACEEVENT_H
#define TRACEEVENT_H

#include <QString>
#include <QDebug>

#include "tracetime.h"

class TraceEvent
{
    TraceEvent();

public:
    QString threadName() const { return m_threadName; }
    pid_t pid() const { return m_pid; }
    int cpuNumber() const { return m_cpu; }
    TraceTime timestamp() const { return m_timestamp; }
    QString eventName() const { return m_eventName; }
    QString details() const { return m_details; }
    bool isValid() const;
    QMap<QString, QString> parameters() const;

    static TraceEvent fromString(const QByteArray &string);
private:
    QString m_threadName;
    pid_t m_pid;
    int m_cpu;
    TraceTime m_timestamp;
    QString m_eventName;
    QString m_details;
    QMap<QString, QString> m_parameters;
};

QDebug operator<<(QDebug dbg, const TraceEvent &event);

#endif // TRACEEVENT_H
