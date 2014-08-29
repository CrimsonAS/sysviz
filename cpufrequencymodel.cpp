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
#if 0
    for (int i = 0; i < m_slices.count(); ++i) {
        qDebug().nospace() << "Slice start " << i << m_slices.at(i)->startTime().tv_sec << "." << m_slices.at(i)->startTime().tv_usec
            << " slice end " << m_slices.at(i)->endTime().tv_sec << "." << m_slices.at(i)->endTime().tv_usec 
            << " freq " << m_slices.at(i)->frequency();
    }
#endif
    return m_slices.count();
}

QVariant CpuFrequencyModel::data(const QModelIndex &index, int role) const
{
}

void CpuFrequencyModel::changeFrequency(timeval time, int frequency)
{
    if (m_currentSlice) {
        if (frequency == m_currentSlice->frequency())
            return; // no point ending it yet

        m_currentSlice->setEndTime(time);
    }

    // TODO: static endtime of 'time' is wrong, it should have a floating "this
    // is current" endtime until we know the real end time..
    m_slices.append(new CpuFrequencySlice(time, time, frequency));
    m_currentSlice = m_slices.last();
}
