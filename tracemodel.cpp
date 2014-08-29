#include <QFile>
#include <QDebug>
#include <QElapsedTimer>

#include "traceevent.h"
#include "tracemodel.h"
#include "cpufrequencymodel.h"

#include <sys/time.h>

TraceModel::TraceModel()
{
    m_earliestEvent.tv_sec = std::numeric_limits<long>::max();
#if defined(Q_OS_MAC)
    m_earliestEvent.tv_usec = std::numeric_limits<suseconds_t>::max();
#else
    m_earliestEvent.tv_usec = std::numeric_limits<long>::max();
#endif

    m_latestEvent.tv_sec = std::numeric_limits<long>::min();
#if defined(Q_OS_MAC)
    m_latestEvent.tv_usec = std::numeric_limits<suseconds_t>::max();
#else
    m_latestEvent.tv_usec = std::numeric_limits<long>::min();
#endif
}

TraceModel *TraceModel::fromFile(QFile *f)
{
    TraceModel *tm = new TraceModel;

    QElapsedTimer fileTimer;
    fileTimer.start();

    while (!f->atEnd()) {
        QByteArray line = f->readLine();
        TraceEvent te = TraceEvent::fromString(line);
        tm->addEvent(te);
    }

    qDebug() << "File processed in " << fileTimer.elapsed();
    qDebug().nospace() << "Earliest event: " << tm->m_earliestEvent.tv_sec << "." << tm->m_earliestEvent.tv_usec;
    qDebug().nospace() << "Latest event: " << tm->m_latestEvent.tv_sec << "." << tm->m_latestEvent.tv_usec;
    return tm;
}

void TraceModel::addEvent(const TraceEvent &te)
{
    if (!te.isValid())
        return;

    timeval ts = te.timestamp();

    if (m_earliestEvent.tv_sec > ts.tv_sec ||
        (m_earliestEvent.tv_sec == ts.tv_sec && m_earliestEvent.tv_usec > ts.tv_usec)) {

        // event time should monotonically increase
        Q_ASSERT(m_earliestEvent.tv_sec == std::numeric_limits<long>::max());

        m_earliestEvent = ts;
    }

    if (m_latestEvent.tv_sec < ts.tv_sec)
        m_latestEvent = ts;
    else if (m_latestEvent.tv_sec == ts.tv_sec && m_latestEvent.tv_usec < ts.tv_usec)
        m_latestEvent = ts;

    if (te.eventName() == "cpu_idle") {
        // Events look like:
        // TraceEvent(0 117925.355823 "<idle>" 1 "cpu_idle" "state=3 cpu_id=1")

        // params looks like:
        // QMap(("cpu_id", "1")("state", "0"))
    } else if (te.eventName() == "cpu_frequency") {
        // Events look like:
        // TraceEvent(17742 117943.296614 "kworker/1:0" 1 "cpu_frequency" "state=918000 cpu_id=1")

        // params looks like:
        // QMap(("cpu_id", "1")("state", "918000"))
        int cpuid = te.parameters()["cpu_id"].toInt(); // TODO: errcheck
        while (cpuid >= cpuCount()) {
            qDebug() << "Creating new CPU frequency model";
            m_cpuFrequencyModels.append(new CpuFrequencyModel(this));
        }

        m_cpuFrequencyModels.at(cpuid)->changeFrequency(te.timestamp(), te.parameters()["state"].toInt() /* TODO: errcheck */);
    } else if (te.eventName() == "kgsl_pwrlevel") {
        // Events look like:
        // TraceEvent(15024 117918.600719 "kworker/u:2" 0 "kgsl_pwrlevel" "d_name=kgsl-3d0 pwrlevel=0 freq=450000000")

        // params looks like:
        // QMap(("d_name", "kgsl-3d0")("freq", "450000000")("pwrlevel", "0"))
    } else if (te.eventName() == "block_rq_issue") {
        // TODO: parse later
    } else if (te.eventName() == "block_rq_complete") {
        // TODO: parse later
    } else if (te.eventName() == "sched_switch") {
        // TODO: parse later
    } else if (te.eventName() == "sched_wakeup") {
        // TODO: parse later
    } else if (te.eventName() == "workqueue_execute_start") {
        // TODO: parse later
    } else if (te.eventName() == "workqueue_execute_end") {
        // TODO: parse later
    } else if (te.eventName() == "workqueue_queue_work") {
        // TODO: parse later
    } else if (te.eventName() == "workqueue_activate_work") {
        // TODO: parse later
    } else if (te.eventName() == "tracing_mark_write") {
        // Events look like:
        //  TraceEvent(17399 117943.616803 "sailfish-maps" 1 "tracing_mark_write" "B|17399|QSGTR::pAS::lock::graphics")

        // the type of systrace event depends on the first character..
        switch (te.details().at(0).toLatin1()) {
            case 'B':
                // B|17399|QSGTR::pAS::lock::graphics
                // B|pid|<stuff>
                //
                // starts a slice, ended by E
                // the 'stuff' is used to describe the event...
                break;
            case 'E':
                // E
                // ends the most recent B slice
                break;
            case 'S':
                // S|17399|QPixmap::loadFromData::graphics|0xbe9d8e20"
                // S|pid|<stuff>|<uid>
                //
                // starts an asynchronous slice.
                // <stuff> describes the event, <uid> is some unique
                // string identifying this event so it can be determined
                // in F
                break;
            case 'F':
                // F|17399|QPixmap::loadFromData::graphics|0xbe9d8e20
                // same syntactically as S, except ending the event.
                break;
            case 'C':
                // TODO
                qWarning() << "Unhandled counter slice " << te;
                break;
            default:
                qWarning() << "Unknown systrace type " << te;
                break;
        }
    } else {
        qWarning() << "Unhandled event: " << te;
    }
}

int TraceModel::cpuCount() const
{
    // TODO: std::max() on cpufrequency and c-state?
    return m_cpuFrequencyModels.count();
}

CpuFrequencyModel *TraceModel::cpuFrequencyModel(int cpu) const
{
    Q_ASSERT(cpu >= 0 && cpu < cpuCount());

    return m_cpuFrequencyModels.at(cpu);
}

// Thoughts about how to present this...
//
// Slice::startTimeS, Slice::endTimeS (and US variants)
// slice time is relative, that is, TraceModel's earliest event represents "0"
// in slice time
// the real contents of a slice depend on the type of slice.
//
// TraceModel::cpuCount
// TraceModel::cpuModel(int cpu)
//
// CpuModel would contain a bunch of slices for frequency & C-state
//  - C0 (operational, CPU fully on)
//  - C1 (first level of idle)
//  - C2 (second level of idle, slower to wake)
//  - C3 (third state of idle, all clocks stopped)
//
//  CpuCSlice::state (0, 1, 2, 3)
//  CpuFrequencySlice::frequency (what mhz frequency the CPU is at)
//
// TraceModel::gpuCount
// TraceModel::gpuModel(int gpu)
//
// GpuModel contains slices about frequency changes in the GPU
// (e.g. powered by kgsl_pwrlevel on the Jolla)
//
// GpuSlice::frequency (what mhz frequency)
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
//
// ThreadSlice::children (a slice may have children)
//
// TBD: how do we represent asynchronous events? (systrace's S & F)
