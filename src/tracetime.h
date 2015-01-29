/*
 * Copyright (C) 2014 Robin Burchell <robin.burchell@viroteck.net>
 * Copyright (C) 2014 Gunnar Sletta <gunnar@sletta.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef TRACETIME_H
#define TRACETIME_H

// a simple replacement for struct timeval that supports some simple operations
struct TraceTime
{
    long tv_sec;
    qlonglong tv_usec;

    double toDouble() const;
};

inline double TraceTime::toDouble() const
{
    return tv_sec + (tv_usec / 1000000.);
}

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
