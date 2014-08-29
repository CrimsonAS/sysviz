#ifndef TRACEMODEL_H
#define TRACEMODEL_H

#include <QObject>

class QFile;

class TraceModel : public QObject
{
    Q_OBJECT
public:
    static TraceModel *fromFile(QFile *f);

private:
    TraceModel();

    timeval m_earliestEvent;
    timeval m_latestEvent;
};

#endif // TRACEMODEL_H
