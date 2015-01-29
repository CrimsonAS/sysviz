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

#ifndef THREADMODEL_H
#define THREADMODEL_H

#include <QAbstractListModel>
#include <QStack>
#include <QList>

#include "tracetime.h"
#include "slice.h"

class ThreadModelSlice;

class ThreadModelSlice : public Slice
{
public:
    ThreadModelSlice(const TraceTime &startTime, const TraceTime &endTime, const QString &text)
    : Slice(startTime, endTime)
    , m_text(text)
    , m_parent(0)
    {
    }

    QString text() const { return m_text; }

    void setParentSlice(ThreadModelSlice *parent)
    {
        Q_ASSERT(!m_parent);
        m_parent = parent;

        parent->m_children.append(this);
    }
    ThreadModelSlice *parentSlice() const { return m_parent; }

    QVector<ThreadModelSlice *> childSlices() const { return m_children; }

private:
    QString m_text;
    ThreadModelSlice *m_parent;
    QVector<ThreadModelSlice *> m_children;
};

class ThreadModel : public QAbstractListModel
{
    Q_OBJECT
public:
    ThreadModel(QObject *parent, qlonglong pid, const QString &threadName);

    enum ModelRoles {
        StartTimeRole = Qt::UserRole,
        EndTimeRole,
        TextRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

    Q_PROPERTY(QString threadName READ threadName CONSTANT)
    QString threadName() const { return m_threadName; }

    Q_PROPERTY(qlonglong pid READ pid CONSTANT)
    qlonglong pid() const { return m_pid; }

    Q_PROPERTY(int maxStackDepth READ maxStackDepth NOTIFY maxStackDepthChanged)
    int maxStackDepth() const { return m_maxStackDepth; }

    void addDurationSlice(const TraceTime &time, QString text);
    void endDurationSlice(const TraceTime &endTime);

    const QList<ThreadModelSlice *> slices() const { return m_topLevelSlices; }

signals:
    void maxStackDepthChanged();

private:
    qlonglong m_pid;
    QString m_threadName;
    QStack<ThreadModelSlice *> m_currentSliceStack;
    QList<ThreadModelSlice *> m_topLevelSlices;
    int m_maxStackDepth;
    int m_currentStackDepth;
};

#endif
