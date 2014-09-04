#ifndef IOTRAFFICMODEL_H
#define IOTRAFFICMODEL_H

#include <QAbstractListModel>

#include "tracetime.h"

class IOTrafficSlice;

class IOTrafficModel : public QAbstractListModel
{
    Q_OBJECT
public:
    IOTrafficModel(QObject *parent);

    enum ModelRoles {
        FrequencyRole = Qt::UserRole,
        StartTimeRole,
        EndTimeRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

    int recordIOStart(const TraceTime &start);
    void recordIOEnd(const TraceTime &time);


private:
    QVector<IOTrafficSlice *> m_slices;
    IOTrafficSlice *m_currentSlice;
};

#endif
