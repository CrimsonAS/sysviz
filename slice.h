#ifndef SLICE_H
#define SLICE_H

#include <sys/time.h>

class Slice
{
public:
    Slice(timeval startTime, timeval endTime)
    : m_startTime(startTime)
    , m_endTime(endTime)
    {
    }

    timeval startTime() const { return m_startTime; }
    timeval endTime() const { return m_endTime; }

    void setEndTime(timeval end) { m_endTime = end; }

private:
    timeval m_startTime;
    timeval m_endTime;
};

#endif
