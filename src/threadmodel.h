#ifndef THREADMODEL_H
#define THREADMODEL_H

#include <QAbstractListModel>
#include <QStack>
#include <QList>

#include "tracetime.h"
class ThreadModelSlice;

class ThreadModel : public QAbstractListModel
{
    Q_OBJECT
public:
    ThreadModel(QObject *parent, qlonglong pid, const QString &threadName);

    enum ModelRoles {
        FrequencyRole = Qt::UserRole,
        StartTimeRole,
        EndTimeRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

    QString threadName() const { return m_threadName; }
    qlonglong pid() const { return m_pid; }

    void addDurationSlice(const TraceTime &time, QString text);
    void endDurationSlice();

private:
    qlonglong m_pid;
    QString m_threadName;
    QStack<ThreadModelSlice *> m_currentSliceStack;
    QList<ThreadModelSlice *> m_topLevelSlices;
};

#endif
