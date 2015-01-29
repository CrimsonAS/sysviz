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

#ifndef GRAPHITEM_H
#define GRAPHITEM_H

#include <QAbstractListModel>

#include <QQuickItem>


class GraphItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(double scaleFactor READ scaleFactor WRITE setScaleFactor NOTIFY scaleFactorChanged)
    Q_PROPERTY(QAbstractListModel *model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(GraphType graphType READ graphType WRITE setGraphType NOTIFY graphTypeChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

    Q_ENUMS(GraphType)
public:
    GraphItem();

    enum GraphType {
        BarGraph,
        LineStripGraph,
        LinesGraph
    };

    QAbstractListModel *model() const { return m_model; }
    void setModel(QAbstractListModel *model);

    double scaleFactor() const { return m_scale; }
    void setScaleFactor(double scaleFactor);

    GraphType graphType() const { return m_graphType; }
    void setGraphType(GraphType gt);

    QColor color() const { return m_color; }
    void setColor(const QColor &c);

protected:
    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *) Q_DECL_OVERRIDE;

signals:
    void scaleFactorChanged();
    void modelChanged();
    void graphTypeChanged();
    void colorChanged();

private:
    QAbstractListModel *m_model;
    double m_scale;
    GraphType m_graphType;
    QColor m_color;
};

#endif // GRAPHITEM_H
