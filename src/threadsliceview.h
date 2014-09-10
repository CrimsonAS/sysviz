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
    Q_PROPERTY(ThreadModel *model READ model WRITE setModel NOTIFY modelChanged)

public:
    ThreadSliceView();

    QQmlComponent *delegate() const { return m_delegate; }
    void setDelegate(QQmlComponent *component);

    double startTime() const { return m_start; }
    void setStartTime(double startTime);

    double endTime() const { return m_end; }
    void setEndTime(double endTime);

    ThreadModel * model() const { return m_model; }
    void setModel(ThreadModel *model);

protected:
    void updatePolish() Q_DECL_OVERRIDE;

signals:
    void delegateChanged();
    void startTimeChanged();
    void endTimeChanged();
    void modelChanged();

private:
    inline bool inRange(ThreadModelSlice *slice) {
        return slice->startTime().toDouble() < m_end && slice->endTime().toDouble() > m_start;
    }
    void populate();
    void populate(QQmlContext *contxt, ThreadModelSlice *slice, int depth, QVector<QQuickItem *> *childList);
    double heightOfDelegate();

    ThreadModel *m_model;
    QQmlComponent *m_delegate;
    ThreadSliceMap m_visibleSlices;
    double m_start;
    double m_end;
    bool m_rebuild;
};


#endif // THREADSLICEVIEW_H
