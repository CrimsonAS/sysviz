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

#include <QDebug>

#include "iotrafficmodel.h"
#include "slice.h"

class IOTrafficSlice : public Slice
{
public:
    IOTrafficSlice(const TraceTime &startTime, const TraceTime &endTime, int events)
    : Slice(startTime, endTime)
    , m_events(events)
    {
    }

    int events() const { return m_events; }

private:
    int m_events;
};

IOTrafficModel::IOTrafficModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_currentSlice(0)
{
    qDebug() << "Creating IO traffic model";
}

int IOTrafficModel::rowCount(const QModelIndex &parent) const
{
    return m_slices.count();
}

QVariant IOTrafficModel::data(const QModelIndex &index, int role) const
{
    switch (role)
    {
        case IOTrafficModel::FrequencyRole:
            return m_slices.at(index.row())->events();
        case IOTrafficModel::StartTimeRole:
            return m_slices.at(index.row())->startTime().toDouble();
        case IOTrafficModel::EndTimeRole:
            return m_slices.at(index.row())->endTime().toDouble();
    }

    Q_UNREACHABLE();
    return QVariant();
}

QHash<int, QByteArray> IOTrafficModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IOTrafficModel::FrequencyRole] = "frequency";
    roles[IOTrafficModel::StartTimeRole] = "startTime";
    roles[IOTrafficModel::EndTimeRole] = "endTime";
    return roles;
}

int IOTrafficModel::recordIOStart(const TraceTime &start)
{
    int eventCount = 0;
    if (m_currentSlice) {
        // end the current slice
        eventCount = m_currentSlice->events();
        recordIOEnd(start);
    }

    // adding a new event
    eventCount++;

    // TODO: static endtime of 'time' is wrong, it should have a floating "this
    // is current" endtime until we know the real end time..
    m_slices.append(new IOTrafficSlice(start, start, eventCount));
    m_currentSlice = m_slices.last();
    return eventCount;
}

void IOTrafficModel::recordIOEnd(const TraceTime &time)
{
    if (m_currentSlice) {
        m_currentSlice->setEndTime(time);
        m_currentSlice = 0;
    }
}
