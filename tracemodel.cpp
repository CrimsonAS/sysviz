#include <QFile>
#include <QDebug>

#include "traceevent.h"
#include "tracemodel.h"

TraceModel *TraceModel::fromFile(QFile *f)
{
    while (!f->atEnd()) {
        QByteArray line = f->readLine();
        TraceEvent te = TraceEvent::fromString(line);
        if (te.isValid()) {
            if (te.eventName() == "cpu_idle") {
                // Events look like:
                // TraceEvent(0 117925.355823 "<idle>" 1 "cpu_idle" "state=3 cpu_id=1")
                QList<QString> datas = te.details().split(" ");
                QMap<QString, QString> params;

                foreach (const QString &data, datas) {
                    QList<QString> param = data.split("=");
                    if (param.length() == 0) {
                        qWarning() << "Empty param for line " << line;
                        continue;
                    }

                    // state 4294967295 is secretly -1 in an unsigned int which
                    // means "exit the current state back to state 0"
                    if (param[0] == "state" && param[1] == "4294967295")
                        param[1] = "0";

                    params[param[0]] = param[1];
                }

                // params should now look like:
                // QMap(("cpu_id", "1")("state", "0"))
            } else {
                qDebug() << te;
            }
        }
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
