#include <QDebug>
#include <QFile>
#include <QCoreApplication>

#include "tracemodel.h"

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    QFile f("trace.systrace");
    if (!f.open(QIODevice::ReadOnly)) {
        qWarning("Can't open trace");
        return -1;
    }

    TraceModel::fromFile(&f);
    return 0;
}
