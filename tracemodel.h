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
    TraceModel();
    void initFromFile(QFile *f);
    void addEvent(const TraceEvent &te);

    Q_PROPERTY(int cpuCount READ cpuCount NOTIFY cpuCountChanged);
    int cpuCount() const;

    Q_INVOKABLE CpuFrequencyModel *cpuFrequencyModel(int cpu) const;

signals:
    void cpuCountChanged();

private:
    timeval m_earliestEvent;
    timeval m_latestEvent;
    QVector<CpuFrequencyModel *> m_cpuFrequencyModels;
};

#endif // TRACEMODEL_H
