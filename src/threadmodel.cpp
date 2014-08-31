#include <QDebug>

#include "threadmodel.h"
#include "slice.h"

class ThreadModelSlice : public Slice
{
public:
    ThreadModelSlice(const TraceTime &startTime, const TraceTime &endTime, const QString &text)
    : Slice(startTime, endTime)
    , m_text(text)
    , m_parent(0)
    {
    }

    QString text() const { return m_text; }

    void setParentSlice(ThreadModelSlice *parent)
    {
        Q_ASSERT(!m_parent);
        m_parent = parent;

        parent->m_children.append(this);
    }
    ThreadModelSlice *parentSlice() const { return m_parent; }

    QVector<ThreadModelSlice *> childSlices() const { return m_children; }

private:
    QString m_text;
    ThreadModelSlice *m_parent;
    QVector<ThreadModelSlice *> m_children;
};


// XXX: threads have an id, perhaps we should use the tid for unique
// identification, and keep the name for display purposes only
ThreadModel::ThreadModel(QObject *parent, qlonglong pid, const QString &threadName)
    : QAbstractListModel(parent)
    , m_pid(pid)
    , m_threadName(threadName)
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
        // TODO: do any of these roles really make sense? maybe it makes more
        // sense to expose a list of the top level slices (QObject* only) and
        // get all properties off them
        case ThreadModel::StartSecondsRole:
            return (qlonglong)m_topLevelSlices.at(index.row())->startTime().tv_sec;
        case ThreadModel::StartMicroSecondsRole:
            return (qlonglong)m_topLevelSlices.at(index.row())->startTime().tv_usec;
        case ThreadModel::EndSecondsRole:
            return (qlonglong)m_topLevelSlices.at(index.row())->endTime().tv_sec;
        case ThreadModel::EndMicroSecondsRole:
            return (qlonglong)m_topLevelSlices.at(index.row())->endTime().tv_usec;
    }

    Q_UNREACHABLE();
    return QVariant();
}

QHash<int, QByteArray> ThreadModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ThreadModel::StartSecondsRole] = "startSeconds";
    roles[ThreadModel::StartMicroSecondsRole] = "startMicroSeconds";
    roles[ThreadModel::EndSecondsRole] = "endSeconds";
    roles[ThreadModel::EndMicroSecondsRole] = "endMicroSeconds";
    return roles;
}

void ThreadModel::addDurationSlice(const TraceTime &time, QString text)
{
    ThreadModelSlice *tm = new ThreadModelSlice(time, time, text);

    // start of a new top level slice
    if (m_currentSliceStack.count() == 0) {
        m_topLevelSlices.append(tm);
    } else {
        tm->setParentSlice(m_currentSliceStack.top());
    }

    m_currentSliceStack.push(tm);
    qDebug() << "Begun slice, current stack is " << m_currentSliceStack.count() << " items";
}

void ThreadModel::endDurationSlice()
{
    qDebug() << "Ending slice, current stack is " << m_currentSliceStack.count() << " items";
    if (m_currentSliceStack.count() > 0)
        m_currentSliceStack.pop();
}
