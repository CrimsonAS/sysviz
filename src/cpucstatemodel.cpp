#include <QDebug>

#include "cpucstatemodel.h"
#include "slice.h"

class CpuCStateSlice : public Slice
{
public:
    CpuCStateSlice(const TraceTime &startTime, const TraceTime &endTime, int cstate)
    : Slice(startTime, endTime)
    , m_cstate(cstate)
    {
    }

    int cstate() const { return m_cstate; }

private:
    int m_cstate;
};

CpuCStateModel::CpuCStateModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_currentSlice(0)
{
    qDebug() << "Creating new CPU C-state model";
}

int CpuCStateModel::rowCount(const QModelIndex &parent) const
{
#if 0
    for (int i = 0; i < m_slices.count(); ++i) {
        qDebug().nospace() << "Slice start " << i << m_slices.at(i)->startTime().tv_sec << "." << m_slices.at(i)->startTime().tv_usec
            << " slice end " << m_slices.at(i)->endTime().tv_sec << "." << m_slices.at(i)->endTime().tv_usec 
            << " freq " << m_slices.at(i)->cstate();
    }
#endif
    return m_slices.count();
}

QVariant CpuCStateModel::data(const QModelIndex &index, int role) const
{
    switch (role)
    {
        case CpuCStateModel::CStateRole:
            return m_slices.at(index.row())->cstate();
        case CpuCStateModel::StartSecondsRole:
            return (qlonglong)m_slices.at(index.row())->startTime().tv_sec;
        case CpuCStateModel::StartMicroSecondsRole:
            return (qlonglong)m_slices.at(index.row())->startTime().tv_usec;
        case CpuCStateModel::EndSecondsRole:
            return (qlonglong)m_slices.at(index.row())->endTime().tv_sec;
        case CpuCStateModel::EndMicroSecondsRole:
            return (qlonglong)m_slices.at(index.row())->endTime().tv_usec;
    }

    Q_UNREACHABLE();
    return QVariant();
}

QHash<int, QByteArray> CpuCStateModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[CpuCStateModel::CStateRole] = "cstate";
    roles[CpuCStateModel::StartSecondsRole] = "startSeconds";
    roles[CpuCStateModel::StartMicroSecondsRole] = "startMicroSeconds";
    roles[CpuCStateModel::EndSecondsRole] = "endSeconds";
    roles[CpuCStateModel::EndMicroSecondsRole] = "endMicroSeconds";
    return roles;
}

void CpuCStateModel::changeCState(const TraceTime &time, int cstate)
{
    if (m_currentSlice) {
        if (cstate == m_currentSlice->cstate())
            return; // no point ending it yet

        m_currentSlice->setEndTime(time);
    }

    // TODO: static endtime of 'time' is wrong, it should have a floating "this
    // is current" endtime until we know the real end time..
    m_slices.append(new CpuCStateSlice(time, time, cstate));
    m_currentSlice = m_slices.last();
}
