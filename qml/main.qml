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

    Text {
        id: statusBar
        color: "white"
        font.pixelSize: 0.2 * cm;
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 0.1 * cm
    }

    TraceView {
        id: view;
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: header.bottom
        anchors.bottom: statusBar.top
        anchors.bottomMargin: 0.1 * cm

        onDisplayToolTip: {
            statusBar.text = text;
        }
        onHideToolTip: {
            statusBar.text = "";
        }
    }





    focus: true
    Keys.onPressed: {
        if (event.key == Qt.Key_Up || event.key == Qt.Key_W) {
            view.startZoomAnimation(view.pps * 2.0);
        } else if (event.key == Qt.Key_Down || event.key == Qt.Key_S) {
            view.startZoomAnimation(view.pps * 0.5);
        } else if (event.key == Qt.Key_Left || event.key == Qt.Key_A) {
            view.startMoveAnimation(-header.width * 0.2);
        } else if (event.key == Qt.Key_Right || event.key == Qt.Key_D) {
            view.startMoveAnimation(header.width * 0.2);
        }
    }

}
