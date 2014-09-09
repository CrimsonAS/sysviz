#ifndef TRACEVIEW_H
#define TRACEVIEW_H

#include <QHash>

#include <QQuickItem>

#include "threadmodel.h"

class TraceModel;
class TraceView;

typedef QHash<ThreadModelSlice *, QVector<QQuickItem *> > ThreadSliceMap;

class ThreadSubView : public QObject
{
    Q_OBJECT
public:
    ThreadSubView(TraceView *view, ThreadModel *);

    void setDelegate(QQmlComponent *component);
    QQmlComponent *delegate() const { return m_delegate; }

    void setSliceParent(QQuickItem *item);
    QQuickItem *sliceParent() const { return m_sliceParent; }

    void populate();

    void setRange(double start, double end);
    void setSliceHeight(double height) { m_sliceHeight = height; }

    inline bool inRange(const TraceTime &start, const TraceTime &end) {
        return start.toDouble() < m_end && end.toDouble() > m_start;
    }

private:
    void populate(QQmlContext *contxt, ThreadModelSlice *slice, int depth, QVector<QQuickItem *> *childList);
    TraceView *m_view;
    ThreadModel *m_model;

    QQuickItem *m_sliceParent;
    QQmlComponent *m_delegate;

    ThreadSliceMap m_visibleSlices;

    double m_start;
    double m_end;
    double m_sliceHeight;

    bool m_rebuild;
};

class TraceView : public QQuickItem
{
    Q_OBJECT

public:
    explicit TraceView(QQuickItem *parent = 0);

    Q_PROPERTY(QQmlComponent *threadSliceDelegate READ threadSliceDelegate WRITE setThreadSliceDelegate NOTIFY threadSliceDelegateChanged)
    QQmlComponent *threadSliceDelegate() const { return m_threadSliceDelegate; }
    void setThreadSliceDelegate(QQmlComponent *delegate);

    Q_PROPERTY(QQmlComponent *rowBackgroundDelegate READ rowBackgroundDelegate WRITE setRowBackgroundDelegate NOTIFY rowBackgroundDelegateChanged)
    QQmlComponent *rowBackgroundDelegate() const { return m_rowBackgroundDelegate; }
    void setRowBackgroundDelegate(QQmlComponent *delegate);

    Q_PROPERTY(TraceModel *model READ model WRITE setModel NOTIFY modelChanged)
    TraceModel *model() const { return m_model; }
    void setModel(TraceModel *model);

    Q_INVOKABLE void recalibrate();
    Q_INVOKABLE void reset();

protected:
    void updatePolish();

signals:
    void threadSliceDelegateChanged();
    void rowBackgroundDelegateChanged();
    void modelChanged();
    void viewportRootChanged();

private:
    void rebuildThreadSlices();
    void rebuildLabels();

    TraceModel *m_model;

    QQmlComponent *m_threadSliceDelegate;
    QQmlComponent *m_rowBackgroundDelegate;

    QHash<ThreadModel *, ThreadSubView *> m_threads;

    bool m_nukeContent;
    bool m_rebuildThreadSlices;
};


#endif // TRACEVIEW_H
