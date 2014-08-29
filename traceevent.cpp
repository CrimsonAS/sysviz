#include <QRegularExpression>

#include "traceevent.h"

TraceEvent::TraceEvent()
    : m_pid(0)
    , m_cpu(0)
{
    m_timestamp.tv_sec = 0;
    m_timestamp.tv_usec = 0;
}

bool TraceEvent::isValid() const
{
    return !m_eventName.isEmpty();
}

TraceEvent TraceEvent::fromString(const QByteArray &line)
{
    TraceEvent te;

    // Matches the trace record in 3.2 and later with the print-tgid option:
    //          <idle>-0    0 [001] d...  1.23: sched_switch
    //
    // A TGID (Thread Group ID) is basically what the Linux kernel calls what
    // userland refers to as a process ID (as opposed to a Linux pid, which is
    // what userland calls a thread ID).
    static QRegularExpression lineREWithTGID(
       "^\\s*(.+)-(\\d+)\\s+\\(\\s*(\\d+|-+)\\)\\s\\[(\\d+)\\]"
       "\\s+[dX.][N.][Hhs.][0-9a-f.]"
       "\\s+(\\d+\\.\\d+):\\s+(\\S+):\\s(.*)$");

    {
        QRegularExpressionMatch m = lineREWithTGID.match(line);
        if (m.hasMatch()) {
            qFatal("I don't know how to parse TGID lines yet");

#if 0
132       threadName: groups[1], 
133       pid: groups[2], 
134       tgid: tgid, 
135       cpuNumber: groups[4], 
136       timestamp: groups[5], 
137       eventName: groups[6], 
138       details: groups[7]
#endif
        }
    }

    static QRegularExpression lineREWithIRQInfo(
       "^\\s*(.+)-(\\d+)\\s+\\[(\\d+)\\]"
       "\\s+[dX.][N.][Hhs.][0-9a-f.]"
       "\\s+(\\d+\\.\\d+):\\s+(\\S+):\\s(.*)$");

    {
        QRegularExpressionMatch m = lineREWithIRQInfo.match(line);
        if (m.hasMatch()) {
            te.m_threadName = m.captured(1);
            te.m_pid = m.captured(2).toInt();
            te.m_cpu = m.captured(3).toInt();

            QString time = m.captured(4);

            int dotpos = time.indexOf('.');
            if (dotpos == -1)
                qFatal("Malformed time %s", qPrintable(time));

            QStringRef seconds(&time, 0, dotpos);
            QStringRef us(&time, dotpos + 1, seconds.size());

            te.m_timestamp.tv_sec = seconds.toUInt();
            te.m_timestamp.tv_usec = us.toUInt();

            te.m_eventName = m.captured(5);
            te.m_details = m.captured(6);
        }
    }

    static QRegularExpression lineREWithLegacyFmt("^\\s*(.+)-(\\d+)\\s+\\[(\\d+)\\]\\s*(\\d+\\.\\d+):\\s+(\\S+):\\s(.*)$");

    {
        QRegularExpressionMatch m = lineREWithLegacyFmt.match(line);
        if (m.hasMatch()) {
            qFatal("I don't know how to parse legacy lines yet");
#if 0
175       threadName: groups[1],
176       pid: groups[2],
177       cpuNumber: groups[3],
178       timestamp: groups[4],
179       eventName: groups[5],
180       details: groups[6]
#endif
        }
    }

    return te;
}

QMap<QString, QString> TraceEvent::parameters() const
{
    if (!m_parameters.isEmpty())
        return m_parameters;

    QList<QString> datas = details().split(" ");
    foreach (const QString &data, datas) {
        QList<QString> param = data.split("=");
        if (param.length() < 2) {
            qWarning() << "Bad param for line " << this;
            continue;
        }


        if (eventName() == "cpu_idle") {
            // state 4294967295 is secretly -1 in an unsigned int which
            // means "exit the current state back to state 0"
            if (param[0] == "state" && param[1] == "4294967295")
                param[1] = "0";
        }

        const_cast<TraceEvent*>(this)->m_parameters[param[0]] = param[1];
    }

    return m_parameters;
}

QDebug operator<<(QDebug dbg, const TraceEvent &event)
{
    dbg.nospace() << "TraceEvent(" << event.pid();
    dbg.space();
    dbg.nospace() << event.timestamp().tv_sec;
    dbg.nospace() << "." << event.timestamp().tv_usec;
    dbg.space() << event.threadName() << event.cpuNumber() << event.eventName();
    dbg.nospace() << event.details() << ")";
    return dbg.maybeSpace();
}

