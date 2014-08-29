#include <QFile>
#include <QDebug>

#include "traceevent.h"
#include "tracemodel.h"

TraceModel *TraceModel::fromFile(QFile *f)
{
    while (!f->atEnd()) {
        QByteArray line = f->readLine();
        TraceEvent te = TraceEvent::fromString(line);
        if (te.isValid())
            qDebug() << te;
    }

    return new TraceModel;
}

// Thoughts about how to present this...
//
// TraceModel::cpuCount
// TraceModel::cpuModel(int cpu)
//
// CpuModel would contain a bunch of slices for frequency, maybe C-state if it's
// useful..
//  - C0 (operational, CPU fully on)
//  - C1 (first level of idle)
//  - C2 (second level of idle, slower to wake)
//  - C3 (third state of idle, all clocks stopped)
//
// TraceModel::processCount
// TraceModel::processModel(int process)
//
// ProcessModel::name, pid (etc)
// ProcessModel::threadCount
// ProcessModel::threadModel(int thread)
//
// ThreadModel::name
//
// A ThreadModel would contain a bunch of slices
