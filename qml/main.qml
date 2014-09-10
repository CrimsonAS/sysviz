import QtQuick 2.2
import SysViz 1.0

import "theme.js" as Theme;

Rectangle {
    id: root

    width: 800;
    height: 600;

    property var rowHeight: 1 * cm;

    gradient: Gradient {
        GradientStop { position: 0; color: Qt.hsla(0.66, 0.2, 0.4); }
        GradientStop { position: 1; color: Qt.hsla(0, 0, 0.2); }
    }

    Header {
        id: header;

        x: view.labelWidth;
        width: view.viewportWidth;
        height: Math.floor(0.5 * cm);

        pps: view.pps;
        startTime: view.startTime
    }

    TraceView {
        id: view;
        anchors.fill: parent
        anchors.topMargin: header.height
    }


    focus: true
    Keys.onPressed: {
        if (event.key == Qt.Key_Up || event.key == Qt.Key_W) {
            view.startZoomAnimation(view.pps * 2.0);
        } else if (event.key == Qt.Key_Down || event.key == Qt.Key_S) {
            view.startZoomAnimation(view.pps * 0.5);
        } else if (event.key == Qt.Key_Left || event.key == Qt.Key_A) {
            view.startMoveAnimation(-header.width * 0.33);
        } else if (event.key == Qt.Key_Right || event.key == Qt.Key_D) {
            view.startMoveAnimation(header.width * 0.33);
        }
    }

}
