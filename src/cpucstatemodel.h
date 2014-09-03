#ifndef CPUCSTATEMODEL_H
#define CPUCSTATEMODEL_H

#include <QAbstractListModel>

#include "tracetime.h"

class CpuCStateSlice;

class CpuCStateModel : public QAbstractListModel
{
    Q_OBJECT
public:
    CpuCStateModel(QObject *parent);

    enum ModelRoles {
        CStateRole = Qt::UserRole,
        StartTimeRole,
        EndTimeRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

    void changeCState(const TraceTime &time, int CState);

private:
    QVector<CpuCStateSlice *> m_slices;
    CpuCStateSlice *m_currentSlice;
};

#endif
