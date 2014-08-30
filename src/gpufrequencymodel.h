#ifndef GPUFREQUENCYMODEL_H
#define GPUFREQUENCYMODEL_H

#include <QAbstractListModel>

#include "tracetime.h"

class GpuFrequencySlice;

class GpuFrequencyModel : public QAbstractListModel
{
    Q_OBJECT
public:
    GpuFrequencyModel(QObject *parent);

    enum ModelRoles {
        FrequencyRole = Qt::UserRole,
        StartSecondsRole,
        StartMicroSecondsRole,
        EndSecondsRole,
        EndMicroSecondsRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

    void changeFrequency(const TraceTime &time, int frequency);

private:
    QVector<GpuFrequencySlice *> m_slices;
    GpuFrequencySlice *m_currentSlice;
};

#endif
