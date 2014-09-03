#include <QFile>
#include <QDebug>
#include <QElapsedTimer>
#include <QtQml>

#include "traceevent.h"
#include "tracemodel.h"
#include "cpucstatemodel.h"
#include "cpufrequencymodel.h"
#include "gpufrequencymodel.h"
#include "processmodel.h"
#include "threadmodel.h"

#include <sys/time.h>

TraceModel::TraceModel()
    : m_gpuFrequencyModel(0)
    , m_processModel(new ProcessModel(this))
{
    qmlRegisterType<CpuFrequencyModel>();
    qmlRegisterType<CpuCStateModel>();
    qmlRegisterType<GpuFrequencyModel>();
    qmlRegisterType<ProcessModel>();
    qmlRegisterType<ThreadModel>();

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

    QFile f("trace.systrace");
    if (!f.open(QIODevice::ReadOnly)) {
        qWarning("Can't open trace");
        return;
    }

    initFromFile(&f);
}

void TraceModel::initFromFile(QFile *f)
{
    QElapsedTimer fileTimer;
    fileTimer.start();

    while (!f->atEnd()) {
        QByteArray line = f->readLine();
        TraceEvent te = TraceEvent::fromString(line);
        addEvent(te);
    }

    qDebug() << "File processed in " << fileTimer.elapsed();
    qDebug().nospace() << "Earliest event: " << m_earliestEvent.tv_sec << "." << m_earliestEvent.tv_usec;
    qDebug().nospace() << "Latest event: " << m_latestEvent.tv_sec << "." << m_latestEvent.tv_usec;

    qDebug() << "Model represents " << cpuCount() << " CPUs";

    for (int i = 0; i < cpuCount(); ++i)
        qDebug() << "Frequency model for CPU ID " << i << " has " << cpuFrequencyModel(i)->rowCount(QModelIndex()) << " slices";
    for (int i = 0; i < cpuCount(); ++i)
        qDebug() << "C-state model for CPU ID " << i << " has " << cpuCStateModel(i)->rowCount(QModelIndex()) << " slices";
    qDebug() << "GPU frequency model has " << gpuFrequencyModel()->rowCount(QModelIndex()) << " slices";
    qDebug() << "Process model has " << m_processModel->rowCount(QModelIndex()) << " threads";
    for (int i = 0; i < m_processModel->rowCount(QModelIndex()); ++i) {
        ThreadModel *tm = qvariant_cast<ThreadModel *>(m_processModel->data(m_processModel->index(i, 0), ProcessModel::ThreadModelRole));
        qDebug() << "Thread " << tm->threadName() << " for PID " << tm->pid() << " has " << tm->rowCount(QModelIndex()) << " slices";
    }
}

void TraceModel::addEvent(const TraceEvent &te)
{
    if (!te.isValid())
        return;

    TraceTime ts = te.timestamp();

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
        int cpuid = te.parameters()["cpu_id"].toInt(); // TODO: errcheck
        while (cpuid >= m_cpuCStateModels.count()) {
            m_cpuCStateModels.append(new CpuCStateModel(this));
            emit cpuCountChanged();
        }

        m_cpuCStateModels.at(cpuid)->changeCState(te.timestamp() - m_earliestEvent, te.parameters()["state"].toInt() /* TODO: errcheck */);
    } else if (te.eventName() == "cpu_frequency") {
        // Events look like:
        // TraceEvent(17742 117943.296614 "kworker/1:0" 1 "cpu_frequency" "state=918000 cpu_id=1")

        // params looks like:
        // QMap(("cpu_id", "1")("state", "918000"))
        int cpuid = te.parameters()["cpu_id"].toInt(); // TODO: errcheck
        while (cpuid >= m_cpuFrequencyModels.count()) {
            m_cpuFrequencyModels.append(new CpuFrequencyModel(this));
            emit cpuCountChanged();
        }

        m_cpuFrequencyModels.at(cpuid)->changeFrequency(te.timestamp() - m_earliestEvent, te.parameters()["state"].toInt() /* TODO: errcheck */);
    } else if (te.eventName() == "kgsl_pwrlevel") {
        // Events look like:
        // TraceEvent(15024 117918.600719 "kworker/u:2" 0 "kgsl_pwrlevel" "d_name=kgsl-3d0 pwrlevel=0 freq=450000000")

        // params looks like:
        // QMap(("d_name", "kgsl-3d0")("freq", "450000000")("pwrlevel", "0"))
        // XXX: we're currently only assuming a single GPU. this is not correct.
        if (!m_gpuFrequencyModel) {
            m_gpuFrequencyModel = new GpuFrequencyModel(this);
        }

        // TODO: what does the 'pwrlevel' mean?
        int khz = te.parameters()["freq"].toInt(); /* TODO: errcheck */
        khz = khz / 1000;
        m_gpuFrequencyModel->changeFrequency(te.timestamp() - m_earliestEvent, khz);
    } else if (te.eventName() == "block_rq_issue") {
        // TODO: parse later
        static bool warned = false;
        if (!warned) {
            qWarning() << "I don't yet know how to handle event of type " << te.eventName();
            warned = true;
        }
    } else if (te.eventName() == "block_rq_complete") {
        // TODO: parse later
        static bool warned = false;
        if (!warned) {
            qWarning() << "I don't yet know how to handle event of type " << te.eventName();
            warned = true;
        }
    } else if (te.eventName() == "sched_switch") {
        // TODO: parse later
        static bool warned = false;
        if (!warned) {
            qWarning() << "I don't yet know how to handle event of type " << te.eventName();
            warned = true;
        }
    } else if (te.eventName() == "sched_wakeup") {
        // TODO: parse later
        static bool warned = false;
        if (!warned) {
            qWarning() << "I don't yet know how to handle event of type " << te.eventName();
            warned = true;
        }
    } else if (te.eventName() == "workqueue_execute_start") {
        // TODO: parse later
        static bool warned = false;
        if (!warned) {
            qWarning() << "I don't yet know how to handle event of type " << te.eventName();
            warned = true;
        }
    } else if (te.eventName() == "workqueue_execute_end") {
        // TODO: parse later
        static bool warned = false;
        if (!warned) {
            qWarning() << "I don't yet know how to handle event of type " << te.eventName();
            warned = true;
        }
    } else if (te.eventName() == "workqueue_queue_work") {
        // TODO: parse later
        static bool warned = false;
        if (!warned) {
            qWarning() << "I don't yet know how to handle event of type " << te.eventName();
            warned = true;
        }
    } else if (te.eventName() == "workqueue_activate_work") {
        // TODO: parse later
        static bool warned = false;
        if (!warned) {
            qWarning() << "I don't yet know how to handle event of type " << te.eventName();
            warned = true;
        }
    } else if (te.eventName() == "tracing_mark_write") {
        addSystraceEvent(te);
    } else {
        qWarning() << "Unhandled event: " << te;
    }
}

void TraceModel::addSystraceEvent(const TraceEvent &te)
{
    // Events look like:
    //  TraceEvent(17399 117943.616803 "sailfish-maps" 1 "tracing_mark_write" "B|17399|QSGTR::pAS::lock::graphics")
    static QMap<pid_t, pid_t> ppids;
    pid_t ppid = te.pid();
    QList<QString> fields = te.details().split('|');

    // the type of systrace event depends on the first character..
    switch (fields[0][0].toLatin1()) {
        case 'B': {
            // B|17399|QSGTR::pAS::lock::graphics
            // B|pid|<stuff>
            //
            // starts a slice, ended by E
            // the 'stuff' is used to describe the event...
            ppid = fields[1].toLongLong(); // TODO: errcheck
            ppids[te.pid()] = ppid;
            ThreadModel *tmodel = m_processModel->ensureThread(ppid, te.threadName());
            tmodel->addDurationSlice(te.timestamp() - m_earliestEvent, fields[2]); // TODO: fields[2] should be a join of fields[2-and-up] in case the field contains a | we split on
            break;
        }
        case 'E': {
            // E
            // ends the most recent B slice
            QMap<pid_t, pid_t>::ConstIterator it = ppids.find(te.pid());
            if (it == ppids.constEnd()) {
                // no corresponding B
                return;
            }
            ppid = *it;
            ThreadModel *tmodel = m_processModel->ensureThread(ppid, te.threadName());
            tmodel->endDurationSlice();
            break;
        }
        case 'S': {
            // S|17399|QPixmap::loadFromData::graphics|0xbe9d8e20"
            // S|pid|<stuff>|<uid>
            //
            // starts an asynchronous slice.
            // <stuff> describes the event, <uid> is some unique
            // string identifying this event so it can be determined
            // in F
            break;
        }
        case 'F': {
            // F|17399|QPixmap::loadFromData::graphics|0xbe9d8e20
            // same syntactically as S, except ending the event.
            break;
        }
        case 'C': {
            // TODO
            qWarning() << "Unhandled counter slice " << te;
            break;
        }
        default: {
            Q_UNREACHABLE();
            qWarning() << "Unknown systrace type " << te;
            break;
        }
    }
}

int TraceModel::cpuCount() const
{
    return std::max(m_cpuFrequencyModels.count(), m_cpuCStateModels.count());
}

CpuFrequencyModel *TraceModel::cpuFrequencyModel(int cpu) const
{
    Q_ASSERT(cpu >= 0 && cpu < m_cpuFrequencyModels.count());

    return m_cpuFrequencyModels.at(cpu);
}

CpuCStateModel *TraceModel::cpuCStateModel(int cpu) const
{
    Q_ASSERT(cpu >= 0 && cpu < m_cpuCStateModels.count());

    return m_cpuCStateModels.at(cpu);
}

GpuFrequencyModel *TraceModel::gpuFrequencyModel() const
{
    if (!m_gpuFrequencyModel) {
        TraceModel *that = const_cast<TraceModel *>(this);
        that->m_gpuFrequencyModel = new GpuFrequencyModel(that);
    }

    return m_gpuFrequencyModel;
}

ProcessModel *TraceModel::processModel() const
{
    return m_processModel;
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
