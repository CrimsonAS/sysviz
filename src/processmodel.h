#ifndef PROCESSMODEL_H
#define PROCESSMODEL_H

#include <QAbstractListModel>

#include "tracetime.h"

class ProcessModel : public QAbstractListModel
{
    Q_OBJECT
public:
    ProcessModel(QObject *parent, qlonglong pid);

    enum ModelRoles {
        PidRole = Qt::UserRole,
        ThreadModelRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

private:
//    QVector<ThreadModel *> m_slices;
    qlonglong m_pid;
};

#endif
