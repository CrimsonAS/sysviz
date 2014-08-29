#include <QDebug>

#include "cpufrequencymodel.h"
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

CpuFrequencyModel::CpuFrequencyModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_currentSlice(0)
{
}

int CpuFrequencyModel::rowCount(const QModelIndex &parent) const
{
    return m_slices.count();
}

QVariant CpuFrequencyModel::data(const QModelIndex &index, int role) const
{
}

void CpuFrequencyModel::changeFrequency(timeval time, int frequency)
{
    if (m_currentSlice) {
        m_currentSlice->setEndTime(time);
    }

    // TODO: static endtime of 'time' is wrong, it should have a floating "this
    // is current" endtime until we know the real end time..
    m_slices.append(new CpuFrequencySlice(time, time, frequency));
    m_currentSlice = m_slices.last();
}
