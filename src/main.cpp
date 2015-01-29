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
