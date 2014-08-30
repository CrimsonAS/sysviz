#ifndef PROCESSMODEL_H
#define PROCESSMODEL_H

#include <QAbstractListModel>

#include "threadmodel.h"

class ProcessModel : public QAbstractListModel
{
    Q_OBJECT
public:
    ProcessModel(QObject *parent);

    enum ModelRoles {
        PidRole = Qt::UserRole,
        ThreadNameRole,
        ThreadModelRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

    ThreadModel *ensureThread(qlonglong pid, const QString &threadName);

private:
    QVector<ThreadModel *> m_threads;
};

#endif
