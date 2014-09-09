#include <QDebug>

#include "threadmodel.h"
#include "slice.h"


// XXX: threads have an id, perhaps we should use the tid for unique
// identification, and keep the name for display purposes only
ThreadModel::ThreadModel(QObject *parent, qlonglong pid, const QString &threadName)
    : QAbstractListModel(parent)
    , m_pid(pid)
    , m_threadName(threadName)
    , m_maxStackDepth(0)
    , m_currentStackDepth(0)
{
}

int ThreadModel::rowCount(const QModelIndex &parent) const
{
    return m_topLevelSlices.count();
}

QVariant ThreadModel::data(const QModelIndex &index, int role) const
{
    switch (role)
    {
        case ThreadModel::StartTimeRole:
            return m_topLevelSlices.at(index.row())->startTime().toDouble();
        case ThreadModel::EndTimeRole:
            return m_topLevelSlices.at(index.row())->endTime().toDouble();
        case ThreadModel::TextRole:
            return m_topLevelSlices.at(index.row())->text();
    }

    Q_UNREACHABLE();
    return QVariant();
}

QHash<int, QByteArray> ThreadModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ThreadModel::StartTimeRole] = "startTime";
    roles[ThreadModel::EndTimeRole] = "endTime";
    return roles;
}

void ThreadModel::addDurationSlice(const TraceTime &time, QString text)
{
    ThreadModelSlice *tm = new ThreadModelSlice(time, time, text);

    // start of a new top level slice
    if (m_currentSliceStack.count() == 0) {
        m_topLevelSlices.append(tm);
        m_currentStackDepth = 1;
    } else {
        tm->setParentSlice(m_currentSliceStack.top());
        m_currentStackDepth++;
    }

    if (m_currentStackDepth > m_maxStackDepth) {
        m_maxStackDepth = m_currentStackDepth;
        qDebug() << "Max stack depth for PID " << pid() << " thread " << threadName() << " is now " << m_maxStackDepth;
        emit maxStackDepthChanged();
   }

    m_currentSliceStack.push(tm);
}

void ThreadModel::endDurationSlice(const TraceTime &endTime)
{
    if (m_currentSliceStack.count() > 0) {
        ThreadModelSlice *tm = m_currentSliceStack.pop();
        tm->setEndTime(endTime);
        m_currentStackDepth--;
    }
}
