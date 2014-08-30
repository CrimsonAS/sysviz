#include <QDebug>

#include "threadmodel.h"
#include "slice.h"

// XXX: threads have an id, perhaps we should use the tid for unique
// identification, and keep the name for display purposes only
ThreadModel::ThreadModel(QObject *parent, const QString &threadName)
    : QAbstractListModel(parent)
    , m_threadName(threadName)
{
}

int ThreadModel::rowCount(const QModelIndex &parent) const
{
    return 0; // XXX:
}

QVariant ThreadModel::data(const QModelIndex &index, int role) const
{
    switch (role)
    {
        case ThreadModel::ThreadNameRole:
            return m_threadName;
    }

    Q_UNREACHABLE();
    return QVariant();
}

QHash<int, QByteArray> ThreadModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ThreadModel::ThreadNameRole] = "name";
    return roles;
}

