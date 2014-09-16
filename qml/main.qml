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

    Text {
        id: statusBar
        color: "white"
        font.pixelSize: 0.25 * cm;
        anchors.baseline: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 0.1 * cm;
        anchors.baselineOffset: - Math.floor(0.15 * cm)
    }

    TraceView {
        id: view;
        width: parent.width
        height: parent.height - statusBar.height - 0.2 * cm - header.height
        y: header.height

        onDisplayToolTip: {
            statusBar.text = text;
        }
        onHideToolTip: {
            statusBar.text = "";
        }
    }

    Item {
        width: view.viewportWidth
        x: view.width - view.viewportWidth
        height: parent.height
        clip: true;

        Header {
            id: header;

            x: 0
            width: parent.width
            height: Math.floor(0.5 * cm);

            pps: view.pps;
            startTime: view.startTime

            viewHeight: view.height
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
