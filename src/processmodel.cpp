#include <QDebug>

#include "processmodel.h"
#include "slice.h"
#include "threadmodel.h"

ProcessModel::ProcessModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int ProcessModel::rowCount(const QModelIndex &parent) const
{
    return m_threads.count();
}

QVariant ProcessModel::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(index.row() >= 0 && index.row() < m_threads.count());
    switch (role)
    {
        case ProcessModel::PidRole:
            return m_threads.at(index.row())->pid();
        case ProcessModel::ThreadNameRole:
            return m_threads.at(index.row())->threadName();
        case ProcessModel::ThreadModelRole:
            return QVariant::fromValue(m_threads.at(index.row()));
    }

    Q_UNREACHABLE();
    return QVariant();
}

QHash<int, QByteArray> ProcessModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ProcessModel::PidRole] = "pid";
    roles[ProcessModel::ThreadNameRole] = "threadName";
    roles[ProcessModel::ThreadModelRole] = "threadModel";
    return roles;
}

ThreadModel *ProcessModel::ensureThread(qlonglong pid, const QString &threadName)
{
    for (int i = 0; i < m_threads.count(); ++i) {
        ThreadModel *tm = m_threads.at(i);
        if (tm->pid() == pid && tm->threadName() == threadName)
            return tm;
    }

    qDebug() << "Creating thread model for " << threadName << " on pid " << pid;
    // TODO: threads should be grouped by process on insertion
    m_threads.append(new ThreadModel(this, pid, threadName));
    return m_threads.last();
}

