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
