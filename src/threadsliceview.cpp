#include "threadsliceview.h"
#include <QQmlContext>
#include <QQmlEngine>

ThreadSliceView::ThreadSliceView()
    : m_model(0)
    , m_delegate(0)
    , m_start(0)
    , m_end(0)
    , m_rebuild(false)
{
    setAcceptHoverEvents(true);
}

void ThreadSliceView::setDelegate(QQmlComponent *component)
{
    if (component == m_delegate)
        return;
    m_delegate = component;
    m_rebuild = true;
    emit delegateChanged();
    polish();
}

void ThreadSliceView::setStartTime(double startTime)
{
    if (m_start == startTime)
        return;
    m_start = startTime;
    emit startTimeChanged();
    polish();
}

void ThreadSliceView::setEndTime(double endTime)
{
    if (m_end == endTime)
        return;
    m_end = endTime;
    emit endTimeChanged();
    polish();
}

void ThreadSliceView::setModel(ThreadModel *model)
{
    if (m_model == model)
        return;
    m_model = model;
    m_rebuild = true;
    emit modelChanged();
    polish();
}

void ThreadSliceView::populate(QQmlContext *context, ThreadModelSlice *slice, int depth, QVector<QQuickItem *> *childList)
{
    QObject *object = m_delegate->create(context);
    QQuickItem *item = qobject_cast<QQuickItem *>(object);
    if (!item) {
        qDebug() << "threadSliceDelegate is not a QQuickItem" << object;
        delete object;
        return;
    }
    item->setProperty("label", slice->text());
    item->setProperty("depth", depth / (double) m_model->maxStackDepth());
    item->setProperty("startTime", slice->startTime().toDouble());
    item->setProperty("endTime", slice->endTime().toDouble());
    item->setY(item->height() * depth);
    item->setParentItem(this);

    foreach (ThreadModelSlice *child, slice->childSlices())
        populate(context, child, depth + 1, childList);

    *childList << item;
}

double ThreadSliceView::heightOfDelegate()
{
    QObject *o = m_delegate->create(QQmlEngine::contextForObject(this));
    double h = qobject_cast<QQuickItem *>(o)->height() * m_model->maxStackDepth();
    delete o;
    return h;
}

void ThreadSliceView::updatePolish()
{
    if (!m_model || !m_delegate) {
        qDebug() << "missing model or delegate" << m_model << m_delegate;
        return;
    }
    if (m_rebuild) {
        foreach (QVector<QQuickItem *> items, m_visibleSlices.values()) {
            qDeleteAll(items);
        }
        m_visibleSlices.clear();
        m_rebuild = false;
    }

    QQmlContext *context = QQmlEngine::contextForObject(this);

    for (ThreadSliceMap::iterator i = m_visibleSlices.begin(); i != m_visibleSlices.end(); ) {
        ThreadModelSlice *s = i.key();
        if (!inRange(s)) {
            qDeleteAll(i.value());
            i = m_visibleSlices.erase(i);
        } else {
            ++i;
        }
    }

    foreach (ThreadModelSlice *slice, m_model->slices()) {
        if (!m_visibleSlices.contains(slice)
                && inRange(slice)) {
            QVector<QQuickItem *> children;
            populate(context, slice, 0, &children);
            m_visibleSlices[slice] = children;
        }
    }

    setHeight(heightOfDelegate());
}

void ThreadSliceView::hoverMoveEvent(QHoverEvent *e)
{
    QPointF pos = e->posF();
    QQuickItem *child = childAt(pos.x(), pos.y());
    if (child && strstr(child->metaObject()->className(), "ThreadSlice"))
        emit hoveringOver(child);
    else
        emit hoveringOver(0);
    QQuickItem::hoverMoveEvent(e);
}


