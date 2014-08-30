#ifndef PROCESSMODEL_H
#define PROCESSMODEL_H

#include <QAbstractListModel>

#include "threadmodel.h"

class ProcessModel : public QAbstractListModel
{
    Q_OBJECT
public:
    ProcessModel(QObject *parent, qlonglong pid);

    enum ModelRoles {
        ThreadModelRole = Qt::UserRole,
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

    void ensureThread(const QString &threadName);

    qlonglong pid() const { return m_pid; }

private:
    QVector<ThreadModel *> m_threads;
    qlonglong m_pid;
};

#endif
