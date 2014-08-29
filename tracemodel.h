#ifndef TRACEMODEL_H
#define TRACEMODEL_H

#include <QObject>

#include "cpufrequencymodel.h"

class QFile;
class TraceEvent;

class TraceModel : public QObject
{
    Q_OBJECT
public:
    static TraceModel *fromFile(QFile *f);
    void addEvent(const TraceEvent &te);

    int cpuCount() const;
    CpuFrequencyModel *cpuFrequencyModel(int cpu) const;

private:
    TraceModel();

    timeval m_earliestEvent;
    timeval m_latestEvent;
    QVector<CpuFrequencyModel *> m_cpuFrequencyModels;
};

#endif // TRACEMODEL_H
