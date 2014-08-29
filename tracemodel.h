#ifndef TRACEMODEL_H
#define TRACEMODEL_H

#include <QObject>

#include "cpufrequencymodel.h"
#include "cpucstatemodel.h"

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
    Q_INVOKABLE CpuCStateModel *cpuCStateModel(int cpu) const;

signals:
    void cpuCountChanged();

private:
    TraceTime m_earliestEvent;
    TraceTime m_latestEvent;
    QVector<CpuFrequencyModel *> m_cpuFrequencyModels;
    QVector<CpuCStateModel *> m_cpuCStateModels;
};

#endif // TRACEMODEL_H
