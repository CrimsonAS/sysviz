#ifndef SLICE_H
#define SLICE_H

#include "tracetime.h"

class Slice
{
public:
    Slice(const TraceTime &startTime, const TraceTime &endTime)
    : m_startTime(startTime)
    , m_endTime(endTime)
    {
    }

    TraceTime startTime() const { return m_startTime; }
    TraceTime endTime() const { return m_endTime; }

    void setEndTime(const TraceTime &end) { m_endTime = end; }

private:
    TraceTime m_startTime;
    TraceTime m_endTime;
};

#endif
