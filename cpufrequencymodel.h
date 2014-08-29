#ifndef CPUFREQUENCYMODEL_H
#define CPUFREQUENCYMODEL_H

#include <QAbstractListModel>

class CpuFrequencySlice;

class CpuFrequencyModel : public QAbstractListModel
{
    Q_OBJECT
public:
    CpuFrequencyModel(QObject *parent);

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

    void changeFrequency(timeval time, int frequency);

private:
    QVector<CpuFrequencySlice *> m_slices;
    CpuFrequencySlice *m_currentSlice;
};

#endif
