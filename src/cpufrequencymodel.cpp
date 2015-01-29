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

#include "cpufrequencymodel.h"
#include "slice.h"

class CpuFrequencySlice : public Slice
{
public:
    CpuFrequencySlice(const TraceTime &startTime, const TraceTime &endTime, int frequency)
    : Slice(startTime, endTime)
    , m_frequency(frequency)
    {
    }

    int frequency() const { return m_frequency; }

private:
    int m_frequency;
};

CpuFrequencyModel::CpuFrequencyModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_currentSlice(0)
{
    qDebug() << "Creating new CPU frequency model";
}

int CpuFrequencyModel::rowCount(const QModelIndex &parent) const
{
#if 0
    for (int i = 0; i < m_slices.count(); ++i) {
        qDebug().nospace() << "Slice start " << i << m_slices.at(i)->startTime().tv_sec << "." << m_slices.at(i)->startTime().tv_usec
            << " slice end " << m_slices.at(i)->endTime().tv_sec << "." << m_slices.at(i)->endTime().tv_usec 
            << " freq " << m_slices.at(i)->frequency();
    }
#endif
    return m_slices.count();
}

QVariant CpuFrequencyModel::data(const QModelIndex &index, int role) const
{
    switch (role)
    {
        case CpuFrequencyModel::FrequencyRole:
            return m_slices.at(index.row())->frequency();
        case CpuFrequencyModel::StartTimeRole:
            return m_slices.at(index.row())->startTime().toDouble();
        case CpuFrequencyModel::EndTimeRole:
            return m_slices.at(index.row())->endTime().toDouble();
    }

    Q_UNREACHABLE();
    return QVariant();
}

QHash<int, QByteArray> CpuFrequencyModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[CpuFrequencyModel::FrequencyRole] = "frequency";
    roles[CpuFrequencyModel::StartTimeRole] = "startTime";
    roles[CpuFrequencyModel::EndTimeRole] = "endTime";
    return roles;
}

void CpuFrequencyModel::changeFrequency(const TraceTime &time, int frequency)
{
    if (m_currentSlice) {
        if (frequency == m_currentSlice->frequency())
            return; // no point ending it yet

        m_currentSlice->setEndTime(time);
    }

    // TODO: static endtime of 'time' is wrong, it should have a floating "this
    // is current" endtime until we know the real end time..
    m_slices.append(new CpuFrequencySlice(time, time, frequency));
    m_currentSlice = m_slices.last();
}
