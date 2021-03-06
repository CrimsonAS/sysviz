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

#ifndef TRACEMODEL_H
#define TRACEMODEL_H

#include <QObject>

class QFile;
class TraceEvent;

#include "cpufrequencymodel.h"
#include "cpucstatemodel.h"
#include "gpufrequencymodel.h"
#include "threadmodel.h"
#include "iotrafficmodel.h"
#include "tracetime.h"

class TraceModel : public QObject
{
    Q_OBJECT
public:

    TraceModel();
    void initFromFile(QFile *f);
    void initFromSystrace(QFile *f);
    void addEvent(const TraceEvent &te);

    Q_PROPERTY(int cpuCount READ cpuCount NOTIFY cpuCountChanged)
    int cpuCount() const;

    Q_PROPERTY(int maxCpuFrequency READ maxCpuFrequency NOTIFY maxCpuFrequencyChanged)
    int maxCpuFrequency() const;

    Q_PROPERTY(int maxGpuFrequency READ maxGpuFrequency NOTIFY maxGpuFrequencyChanged)
    int maxGpuFrequency() const;

    Q_PROPERTY(int maxIOTraffic READ maxIOTraffic NOTIFY maxIOTrafficChanged)
    int maxIOTraffic() const;
    Q_INVOKABLE IOTrafficModel *ioTrafficModel() const;

    Q_PROPERTY(double traceLength READ traceLength NOTIFY traceLengthChanged)
    double traceLength() const { return (m_latestEvent - m_earliestEvent).toDouble(); }

    Q_INVOKABLE CpuFrequencyModel *cpuFrequencyModel(int cpu) const;
    Q_INVOKABLE CpuCStateModel *cpuCStateModel(int cpu) const;

    Q_INVOKABLE GpuFrequencyModel *gpuFrequencyModel() const;

    Q_PROPERTY(int threadCount READ threadCount NOTIFY threadCountChanged)
    int threadCount() const;

    Q_INVOKABLE ThreadModel *threadModel(int threadIdx) const;

signals:
    void cpuCountChanged();
    void traceLengthChanged();
    void maxCpuFrequencyChanged();
    void maxGpuFrequencyChanged();
    void threadCountChanged();
    void maxIOTrafficChanged();

private:
    ThreadModel *ensureThread(qlonglong pid, const QString &threadName);
    void addSystraceEvent(const TraceEvent &te);

private:
    TraceTime m_earliestEvent;
    TraceTime m_latestEvent;
    QVector<CpuFrequencyModel *> m_cpuFrequencyModels;
    QVector<CpuCStateModel *> m_cpuCStateModels;
    GpuFrequencyModel *m_gpuFrequencyModel;
    QVector<ThreadModel *> m_threadModels;
    int m_maxCpuFrequency;
    int m_maxGpuFrequency;
    IOTrafficModel *m_ioTrafficModel;
    int m_maxIOTraffic;
};

#endif // TRACEMODEL_H
