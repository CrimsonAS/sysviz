#ifndef TRACEMODEL_H
#define TRACEMODEL_H

#include <QObject>

class QFile;
class TraceEvent;

#include "cpufrequencymodel.h"
#include "cpucstatemodel.h"
#include "gpufrequencymodel.h"
#include "tracetime.h"
#include "processmodel.h"

class TraceModel : public QObject
{
    Q_OBJECT
public:

    TraceModel();
    void initFromFile(QFile *f);
    void addEvent(const TraceEvent &te);

    Q_PROPERTY(int cpuCount READ cpuCount NOTIFY cpuCountChanged)
    int cpuCount() const;

    Q_PROPERTY(int maxCpuFrequency READ maxCpuFrequency NOTIFY maxCpuFrequencyChanged)
    int maxCpuFrequency() const;

    Q_PROPERTY(int maxGpuFrequency READ maxGpuFrequency NOTIFY maxGpuFrequencyChanged)
    int maxGpuFrequency() const;

    Q_PROPERTY(double traceLength READ traceLength NOTIFY traceLengthChanged)
    double traceLength() const { return (m_latestEvent - m_earliestEvent).toDouble(); }

    Q_INVOKABLE CpuFrequencyModel *cpuFrequencyModel(int cpu) const;
    Q_INVOKABLE CpuCStateModel *cpuCStateModel(int cpu) const;

    Q_INVOKABLE GpuFrequencyModel *gpuFrequencyModel() const;
    Q_INVOKABLE ProcessModel *processModel() const;

signals:
    void cpuCountChanged();
    void traceLengthChanged();
    void maxCpuFrequencyChanged();
    void maxGpuFrequencyChanged();

private:
    void addSystraceEvent(const TraceEvent &te);

private:
    TraceTime m_earliestEvent;
    TraceTime m_latestEvent;
    QVector<CpuFrequencyModel *> m_cpuFrequencyModels;
    QVector<CpuCStateModel *> m_cpuCStateModels;
    GpuFrequencyModel *m_gpuFrequencyModel;
    ProcessModel *m_processModel;
    int m_maxCpuFrequency;
    int m_maxGpuFrequency;
};

#endif // TRACEMODEL_H
