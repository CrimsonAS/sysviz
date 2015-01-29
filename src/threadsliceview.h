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

#ifndef THREADSLICEVIEW_H
#define THREADSLICEVIEW_H

#include <QQuickItem>

#include "threadmodel.h"

typedef QHash<ThreadModelSlice *, QVector<QQuickItem *> > ThreadSliceMap;

class ThreadSliceView : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QQmlComponent *delegate READ delegate WRITE setDelegate NOTIFY delegateChanged)
    Q_PROPERTY(double startTime READ startTime WRITE setStartTime NOTIFY startTimeChanged)
    Q_PROPERTY(double endTime READ endTime WRITE setEndTime NOTIFY endTimeChanged)
    Q_PROPERTY(double minimumSliceLength READ minimumSliceLength WRITE setMinimumSliceLength NOTIFY minimumSliceLengthChanged)
    Q_PROPERTY(ThreadModel *model READ model WRITE setModel NOTIFY modelChanged)

public:
    ThreadSliceView();

    QQmlComponent *delegate() const { return m_delegate; }
    void setDelegate(QQmlComponent *component);

    double startTime() const { return m_start; }
    void setStartTime(double startTime);

    double endTime() const { return m_end; }
    void setEndTime(double endTime);

    double minimumSliceLength() const { return m_minSliceLength; }
    void setMinimumSliceLength(double min);

    ThreadModel * model() const { return m_model; }
    void setModel(ThreadModel *model);

    Q_INVOKABLE void calibrate() { polish(); }

protected:
    void updatePolish() Q_DECL_OVERRIDE;
    void hoverMoveEvent(QHoverEvent *e) Q_DECL_OVERRIDE;

signals:
    void delegateChanged();
    void startTimeChanged();
    void endTimeChanged();
    void modelChanged();
    void minimumSliceLengthChanged();

    void hoveringOver(QQuickItem *item);

private:
    inline bool inRange(ThreadModelSlice *slice) {
        double s = slice->startTime().toDouble();
        double e = slice->endTime().toDouble();
        return s < m_end && e > m_start && (e - s) > m_minSliceLength;
    }
    void populate();
    void populate(QQmlContext *contxt, ThreadModelSlice *slice, int depth, QVector<QQuickItem *> *childList);
    double heightOfDelegate();

    ThreadModel *m_model;
    QQmlComponent *m_delegate;
    ThreadSliceMap m_visibleSlices;
    double m_start;
    double m_end;
    double m_minSliceLength;
    bool m_rebuild;
};


#endif // THREADSLICEVIEW_H
