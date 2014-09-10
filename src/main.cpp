#include <QDebug>
#include <QFile>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml>

#include <QQuickView>
#include <QScreen>

#include "tracemodel.h"
#include "threadsliceview.h"
#include "graphitem.h"

#ifdef QT_DQML_LIB
#include <dqml/dqmllocalserver.h>
#endif

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

//    qmlRegisterType<TraceView>("SysViz", 1, 0, "TraceView");
    qmlRegisterType<ThreadSliceView>("SysViz", 1, 0, "ThreadSliceView");
    qmlRegisterType<GraphItem>      ("SysViz", 1, 0, "GraphItem");
    qmlRegisterType<TraceModel>();
    qmlRegisterType<QAbstractListModel>();
    qmlRegisterType<CpuFrequencyModel>();
    qmlRegisterType<GpuFrequencyModel>();

    TraceModel model;

    QQuickView view;
    QSurfaceFormat format = view.requestedFormat();
    format.setSamples(16);
    view.setFormat(format);

    view.rootContext()->setContextProperty("traceModel", &model);
    view.rootContext()->setContextProperty("cm", view.screen()->physicalDotsPerInch() / 2.54);

    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl::fromLocalFile("qml/main.qml"));
    view.show();

#ifdef QT_DQML_LIB
    DQmlLocalServer server(view.engine(), &view, "qml/main.qml");
    server.fileTracker()->track("qml", "qml");
#endif

    app.exec();
}
