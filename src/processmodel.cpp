#include <QDebug>

#include "processmodel.h"
#include "slice.h"

ProcessModel::ProcessModel(QObject *parent, qlonglong pid)
    : QAbstractListModel(parent)
    , m_pid(pid)
{
}

int ProcessModel::rowCount(const QModelIndex &parent) const
{
    return 0;
//    return m_threads.count();
}

QVariant ProcessModel::data(const QModelIndex &index, int role) const
{
    switch (role)
    {
        case ProcessModel::PidRole:
            return m_pid;
//        case ProcessModel::ThreadModelRole:
//            return m_threads.at(index.row());
    }

    Q_UNREACHABLE();
    return QVariant();
}

QHash<int, QByteArray> ProcessModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ProcessModel::PidRole] = "pid";
//    roles[ProcessModel::ThreadModelRole] = "threadModel";
    return roles;
}

