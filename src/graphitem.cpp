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

#include "graphitem.h"

#include <QSGFlatColorMaterial>
#include <QSGGeometryNode>

GraphItem::GraphItem()
    : m_model(0)
    , m_scale(1)
    , m_graphType(BarGraph)
    , m_color(Qt::white)
{
    setFlag(ItemHasContents);
}

void GraphItem::setModel(QAbstractListModel *model)
{
    if (m_model == model)
        return;

    m_model = model;
    emit modelChanged();
    update();
}

void GraphItem::setScaleFactor(double scale)
{
    if (m_scale == scale)
        return;
    m_scale = scale;
    emit scaleFactorChanged();
    update();
}

void GraphItem::setGraphType(GraphType gt)
{
    if (m_graphType == gt)
        return;
    m_graphType = gt;
    emit graphTypeChanged();
    update();
}

void GraphItem::setColor(const QColor &c)
{
    if (m_color == c)
        return;
    m_color = c;
    emit colorChanged();
    update();
}

QSGNode *GraphItem::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    // Don't bother optimizing.. it won't be called often...
    if (oldNode)
        delete oldNode;

    if (!m_model)
        return 0;

    int count = m_model->rowCount();
    if (count == 0)
        return 0;


    int vertexCount;
    int drawMode;
    if (m_graphType == BarGraph) {
        vertexCount = count * 4;
        drawMode = GL_TRIANGLE_STRIP;
    } else if (m_graphType == LinesGraph) {
        vertexCount = count * 2;
        drawMode = GL_LINES;
    } else {
        vertexCount = count * 2;
        drawMode = GL_LINE_STRIP;
    }

    QSGGeometryNode *node = new QSGGeometryNode();
    QSGFlatColorMaterial *material = new QSGFlatColorMaterial();
    material->setColor(m_color);
    node->setMaterial(material);
    node->setFlag(QSGNode::OwnsMaterial);
    QSGGeometry *g = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), vertexCount);
    g->setDrawingMode(drawMode);
    node->setGeometry(g);
    node->setFlag(QSGNode::OwnsGeometry);

    // Figure out the role names, since model can be either both GPUFrequencyModel
    // and CPUFrequencyModel
    int startRole = -1;
    int endRole = -1;
    int frequencyRole = -1;
    QHash<int, QByteArray> names = m_model->roleNames();
    for (QHash<int, QByteArray>::const_iterator it = names.constBegin(); it != names.constEnd(); ++it) {
        if (it.value() == "startTime") startRole = it.key();
        else if (it.value() == "endTime") endRole = it.key();
        else if (it.value() == "frequency") frequencyRole = it.key();
    }

    QSGGeometry::Point2D *pt = g->vertexDataAsPoint2D();
    for (int i=0; i<count; ++i) {
        double s = m_model->data(m_model->index(i, 0), startRole).toDouble();
        double e = m_model->data(m_model->index(i, 0), endRole).toDouble();
        double f = m_model->data(m_model->index(i, 0), frequencyRole).toDouble();
        if (m_graphType == BarGraph) {
            pt[i*4  ].set(s, 0);
            pt[i*4+1].set(s, f);
            pt[i*4+2].set(e, 0);
            pt[i*4+3].set(e, f);
        } else {
            pt[i*2  ].set(s, f);
            pt[i*2+1].set(e, f);
        }
    }

    return node;
}
