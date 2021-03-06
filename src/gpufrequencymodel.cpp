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

#include "gpufrequencymodel.h"
#include "slice.h"

class GpuFrequencySlice : public Slice
{
public:
    GpuFrequencySlice(const TraceTime &startTime, const TraceTime &endTime, int frequency)
    : Slice(startTime, endTime)
    , m_frequency(frequency)
    {
    }

    int frequency() const { return m_frequency; }

private:
    int m_frequency;
};

GpuFrequencyModel::GpuFrequencyModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_currentSlice(0)
{
    qDebug() << "Creating GPU frequency model";
}

int GpuFrequencyModel::rowCount(const QModelIndex &parent) const
{
    return m_slices.count();
}

QVariant GpuFrequencyModel::data(const QModelIndex &index, int role) const
{
    switch (role)
    {
        case GpuFrequencyModel::FrequencyRole:
            return m_slices.at(index.row())->frequency();
        case GpuFrequencyModel::StartTimeRole:
            return m_slices.at(index.row())->startTime().toDouble();
        case GpuFrequencyModel::EndTimeRole:
            return m_slices.at(index.row())->endTime().toDouble();
    }

    Q_UNREACHABLE();
    return QVariant();
}

QHash<int, QByteArray> GpuFrequencyModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[GpuFrequencyModel::FrequencyRole] = "frequency";
    roles[GpuFrequencyModel::StartTimeRole] = "startTime";
    roles[GpuFrequencyModel::EndTimeRole] = "endTime";
    return roles;
}

void GpuFrequencyModel::changeFrequency(const TraceTime &time, int frequency)
{
    if (m_currentSlice) {
        if (frequency == m_currentSlice->frequency())
            return; // no point ending it yet

        m_currentSlice->setEndTime(time);
    }

    // TODO: static endtime of 'time' is wrong, it should have a floating "this
    // is current" endtime until we know the real end time..
    m_slices.append(new GpuFrequencySlice(time, time, frequency));
    m_currentSlice = m_slices.last();
}
