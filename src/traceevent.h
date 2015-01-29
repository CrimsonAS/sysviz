/*
 * Copyright (C) 2014 Robin Burchell <robin.burchell@viroteck.net>
 * Copyright (C) 2014 Gunnar Sletta <gunnar@sletta.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

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
