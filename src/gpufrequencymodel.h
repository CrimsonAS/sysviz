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

#ifndef GPUFREQUENCYMODEL_H
#define GPUFREQUENCYMODEL_H

#include <QAbstractListModel>

#include "tracetime.h"

class GpuFrequencySlice;

class GpuFrequencyModel : public QAbstractListModel
{
    Q_OBJECT
public:
    GpuFrequencyModel(QObject *parent);

    enum ModelRoles {
        FrequencyRole = Qt::UserRole,
        StartTimeRole,
        EndTimeRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

    void changeFrequency(const TraceTime &time, int frequency);

private:
    QVector<GpuFrequencySlice *> m_slices;
    GpuFrequencySlice *m_currentSlice;
};

#endif
