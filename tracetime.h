#ifndef TRACETIME_H
#define TRACETIME_H

// a simple replacement for struct timeval that supports some simple operations
struct TraceTime
{
    long tv_sec;
    qlonglong tv_usec;
};

inline TraceTime operator+(const TraceTime &a, const TraceTime &b)
{
    TraceTime tmp;

    qlonglong atus = a.tv_sec * 1000 * 1000 + a.tv_usec; // sum up a in us
    qlonglong btus = b.tv_sec * 1000 * 1000 + b.tv_usec; // sum up b in us
    qlonglong total = atus + btus; // get the new total of us (a - b)

    tmp.tv_sec = total / 1000 / 1000; // put the total number of seconds in
    total -= tmp.tv_sec * 1000 * 1000; // take the number of seconds (in us) out of total
    tmp.tv_usec = total; // use the remainder as us

    return tmp;
}

inline TraceTime operator-(const TraceTime &a, const TraceTime &b)
{
    TraceTime tmp;

    qlonglong atus = a.tv_sec * 1000 * 1000 + a.tv_usec; // sum up a in us
    qlonglong btus = b.tv_sec * 1000 * 1000 + b.tv_usec; // sum up b in us
    qlonglong total = atus - btus; // get the new total of us (a - b)

    tmp.tv_sec = total / 1000 / 1000; // put the total number of seconds in
    total -= tmp.tv_sec * 1000 * 1000; // take the number of seconds (in us) out of total
    tmp.tv_usec = total; // use the remainder as us
    return tmp;
}

inline bool operator==(const TraceTime &a, const TraceTime &b)
{
    return a.tv_sec == b.tv_sec &&
           a.tv_usec == b.tv_usec;
}

inline bool operator!=(const TraceTime &a, const TraceTime &b)
{
    return !(a == b);
}

#endif
