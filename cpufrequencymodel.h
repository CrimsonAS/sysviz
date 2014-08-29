#ifndef CPUFREQUENCYMODEL_H
#define CPUFREQUENCYMODEL_H

#include <QAbstractListModel>

#include "slice.h"

class CpuFrequencySlice : public Slice
{
public:
    CpuFrequencySlice(timeval startTime, timeval endTime, int frequency)
    : Slice(startTime, endTime)
    , m_frequency(frequency)
    {
    }

    int frequency() const { return m_frequency; }

private:
    int m_frequency;
};

class CpuFrequencyModel : public QAbstractListModel
{
    Q_OBJECT
public:
    CpuFrequencyModel(QObject *parent);

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

private:
    QVector<CpuFrequencySlice> m_slices;
};

#endif
