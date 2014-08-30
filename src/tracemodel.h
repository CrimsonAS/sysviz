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

    Q_PROPERTY(int cpuCount READ cpuCount NOTIFY cpuCountChanged);
    int cpuCount() const;

    Q_INVOKABLE CpuFrequencyModel *cpuFrequencyModel(int cpu) const;
    Q_INVOKABLE CpuCStateModel *cpuCStateModel(int cpu) const;

    Q_INVOKABLE GpuFrequencyModel *gpuFrequencyModel() const;

signals:
    void cpuCountChanged();

private:
    TraceTime m_earliestEvent;
    TraceTime m_latestEvent;
    QVector<CpuFrequencyModel *> m_cpuFrequencyModels;
    QVector<CpuCStateModel *> m_cpuCStateModels;
    GpuFrequencyModel *m_gpuFrequencyModel;
    QMap<qlonglong, ProcessModel *> m_processModels;
};

#endif // TRACEMODEL_H
