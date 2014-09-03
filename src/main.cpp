#include <QDebug>
#include <QFile>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml>

#include <QQuickView>

#include "tracemodel.h"

#ifdef QT_DQML_LIB
#include <dqml/dqmllocalserver.h>
#endif

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);
//    qmlRegisterType<TraceModel>("SysViz", 1, 0, "TraceModel");

    TraceModel model;

    QQuickView view;

    view.rootContext()->setContextProperty("traceModel", &model);

    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl::fromLocalFile("qml/main.qml"));
    view.show();



#ifdef QT_DQML_LIB
    DQmlLocalServer server(view.engine(), &view, "qml/main.qml");
    server.fileTracker()->track("qml", "qml");
    server.reloadQml();
#endif

    app.exec();
}
