#include "traceview.h"
#include "tracemodel.h"

#include <qmetaobject.h>

#include <QQmlEngine>

bool lookupProperty(QQuickItem *item, const QMetaObject *meta, const char *name, QMetaProperty *property)
{
    int idx = meta->indexOfProperty(name);
    if (!idx) {
        qDebug() << "threadSliceDelegate is missing property:" << name;
        delete item;
        return false;
    }
    *property = meta->property(idx);
    return true;
}

ThreadSubView::ThreadSubView(TraceView *view, ThreadModel *model)
    : m_view(view)
    , m_model(model)
    , m_sliceParent(0)
    , m_start(0)
    , m_end(0)
    , m_sliceHeight(0)
    , m_rebuild(false)
{
}

void ThreadSubView::setRange(double start, double end)
{
    if (m_start == start && m_end == end)
        return;
    m_start = start;
    m_end = end;
    m_rebuild = true;
}

void ThreadSubView::setDelegate(QQmlComponent *d)
{
    if (m_delegate == d)
        return;
    m_delegate = d;
    m_rebuild = true;
}

void ThreadSubView::setSliceParent(QQuickItem *item)
{
    if (m_sliceParent == item)
        return;
    m_sliceParent = item;
    m_rebuild = true;
}

void ThreadSubView::populate(QQmlContext *context, ThreadModelSlice *slice, int depth, QVector<QQuickItem *> *childList)
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
    item->setHeight(m_sliceHeight);
    item->setY(item->height() * depth);

    item->setParentItem(m_sliceParent);

    foreach (ThreadModelSlice *child, slice->childSlices())
        populate(context, child, depth + 1, childList);

    *childList << item;
}

void ThreadSubView::populate()
{
    Q_ASSERT(m_delegate);
    Q_ASSERT(m_sliceParent);

    if (!m_rebuild)
        return;

    QQmlContext *context = QQmlEngine::contextForObject(m_sliceParent);

    for (ThreadSliceMap::iterator i = m_visibleSlices.begin(); i != m_visibleSlices.end(); ) {
        ThreadModelSlice *s = i.key();
        if (!inRange(s->startTime(), s->endTime())) {
            qDeleteAll(i.value());
            i = m_visibleSlices.erase(i);

        } else {
            ++i;
        }
    }

    foreach (ThreadModelSlice *slice, m_model->slices()) {
        if (!m_visibleSlices.contains(slice)
                && inRange(slice->startTime(), slice->endTime())) {
            QVector<QQuickItem *> children;
            populate(context, slice, 0, &children);
            m_visibleSlices[slice] = children;
        }
    }
}

TraceView::TraceView(QQuickItem *parent)
    : QQuickItem(parent)
    , m_model(0)
    , m_threadSliceDelegate(0)
    , m_rowBackgroundDelegate(0)
    , m_nukeContent(false)
    , m_rebuildThreadSlices(false)
{
}

void TraceView::setModel(TraceModel *model)
{
    if (m_model == model)
        return;
    m_model = model;
    m_nukeContent = true;
    emit modelChanged();
    polish();
}

void TraceView::setThreadSliceDelegate(QQmlComponent *delegate)
{
    if (m_threadSliceDelegate == delegate)
        return;
    m_threadSliceDelegate = delegate;
    m_nukeContent = true;
    emit threadSliceDelegateChanged();
    polish();
}

void TraceView::setRowBackgroundDelegate(QQmlComponent *delegate)
{
    if (m_rowBackgroundDelegate == delegate)
        return;
    m_rowBackgroundDelegate = delegate;
    m_nukeContent = true;
    emit rowBackgroundDelegateChanged();
    polish();
}

void TraceView::recalibrate()
{
    m_rebuildThreadSlices = true;
    polish();
}

void TraceView::reset()
{
    m_nukeContent = true;
    polish();
}

void TraceView::updatePolish()
{
    if (!m_rowBackgroundDelegate) {
        qDebug() << "missing rowBackgroundDelegate";
        return;
    }

    bool buildLabels = false;

    if (m_nukeContent) {
        qDeleteAll(m_threads.values());
        m_threads.clear();
        m_rebuildThreadSlices = true;
        m_nukeContent = false;
        buildLabels = true;
    }

    if (m_rebuildThreadSlices)
        rebuildThreadSlices();

    if (buildLabels)
        rebuildLabels();
}

void TraceView::rebuildThreadSlices()
{
    if (!m_threadSliceDelegate) {
        qDebug() << "missing threadSliceDelegate";
        return;
    }

    if (!m_model) {
        qDebug() << "missing model";
        return;
    }

    double threadSliceHeight = property("threadSliceHeight").toDouble();
    if (threadSliceHeight <= 0) {
        qDebug() << "missing threadSliceHeight";
        return;
    }

    QQuickItem *contentRoot = qvariant_cast<QQuickItem *>(property("contentRoot"));
    if (!contentRoot) {
        qDebug() << "bad content root";
        return;
    }

    double rowSpacing = property("rowSpacing").toDouble();

    int tc = m_model->threadCount();
    QQmlContext *context = QQmlEngine::contextForObject(this);

    double y = 0;

    if (m_threads.size() == 0) {
        for (int i=0; i<tc; ++i) {
            QObject *bgObj = m_rowBackgroundDelegate->create(context);
            QQuickItem *bg = qobject_cast<QQuickItem *>(bgObj);
            if (!bg) {
                qDebug() << "rowBackgroundDelegate is not a QQuickItem";
                return;
            }

            ThreadModel *tm = m_model->threadModel(i);
            qDebug() << " - trackng a new thread model..." << tm;
            bg->setPosition(QPointF(0, y));
            bg->setParentItem(contentRoot);
            bg->setHeight(threadSliceHeight * tm->maxStackDepth());

            ThreadSubView *view = new ThreadSubView(this, tm);
            view->setDelegate(m_threadSliceDelegate);
            view->setSliceParent(bg);
            view->setSliceHeight(threadSliceHeight);
            m_threads[tm] = view;

            y += bg->height() + rowSpacing;
        }
    }

    double start = qvariant_cast<double>(property("startTime"));
    double end = qvariant_cast<double>(property("endTime"));

    for (int i=0; i<tc; ++i) {
        ThreadModel *tm = m_model->threadModel(i);
        ThreadSubView *view = m_threads[tm];
        view->setRange(start, end);
        view->populate();
    }

    setProperty("contentHeight", y);
}

void TraceView::rebuildLabels()
{
    qDebug() << "rebuilding labels...";
    QQmlComponent *labelDelegate = qvariant_cast<QQmlComponent *>(property("labelDelegate"));
    if (!labelDelegate) {
        qDebug() << "missing labelDelgate";
        return;
    }
    QQuickItem *labelRoot = qvariant_cast<QQuickItem *>(property("labelRoot"));
    if (!labelRoot) {
        qDebug() << "missing labelRoot";
        return;
    }

    double labelWidth = -1;

    double threadSliceHeight = property("threadSliceHeight").toDouble();
    double rowSpacing = property("rowSpacing").toDouble();
    double y = 0;
    QQmlContext *context = QQmlEngine::contextForObject(this);
    QList<QQuickItem *> labels;
    for (int i=0; i<m_model->threadCount(); ++i) {
        QObject *o = labelDelegate->create(context);
        QQuickItem *label = qobject_cast<QQuickItem *>(o);
        if (!label) {
            qDebug() << "labelDelegate does not produce QQuickItems";
            delete o;
            return;
        }

        ThreadModel *tm = m_model->threadModel(i);
        label->setProperty("text", QString::fromLatin1("PID: %1\n%2")
                           .arg(tm->pid())
                           .arg(tm->threadName()));
        label->setHeight(threadSliceHeight * tm->maxStackDepth());
        label->setY(y);
        label->setParentItem(labelRoot);

        labels << label;
        labelWidth = qMax(labelWidth, label->width());

        y += label->height() + rowSpacing;
    }

    foreach (QQuickItem *label, labels)
        label->setWidth(labelWidth);
    setProperty("labelWidth", labelWidth);


}
