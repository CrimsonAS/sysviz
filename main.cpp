#include <QDebug>
#include <QFile>

#include "traceevent.h"

int main(int argc, char **argv)
{
    QFile f("trace.systrace");
    if (!f.open(QIODevice::ReadOnly)) {
        perror("Can't open trace");
        return -1;
    }

    while (!f.atEnd()) {
        QByteArray line = f.readLine();
        TraceEvent te = TraceEvent::fromString(line);
        if (te.isValid())
            qDebug() << te;
    }

    return 0;
}
