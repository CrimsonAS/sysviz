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
        StartTimeRole = Qt::UserRole,
        EndTimeRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

    Q_PROPERTY(QString threadName READ threadName CONSTANT)
    QString threadName() const { return m_threadName; }

    Q_PROPERTY(qlonglong pid READ pid CONSTANT)
    qlonglong pid() const { return m_pid; }

    Q_PROPERTY(int maxStackDepth READ maxStackDepth NOTIFY maxStackDepthChanged)
    int maxStackDepth() const { return m_maxStackDepth; }

    void addDurationSlice(const TraceTime &time, QString text);
    void endDurationSlice(const TraceTime &endTime);

signals:
    void maxStackDepthChanged();

private:
    qlonglong m_pid;
    QString m_threadName;
    QStack<ThreadModelSlice *> m_currentSliceStack;
    QList<ThreadModelSlice *> m_topLevelSlices;
    int m_maxStackDepth;
    int m_currentStackDepth;
};

#endif
