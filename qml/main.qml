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
        startTime: contentRoot.contentX / pps;
    }

    TraceView {
        id: view

        anchors.fill: parent
        anchors.topMargin: header.height

        property real labelWidth: 3 * cm
        property real pps: 10 * cm;
        property real viewportWidth: width - labelWidth;

        property real startTime: contentRoot.contentX / pps;
        property real endTime: startTime + contentRoot.width / pps;
        property real threadSliceHeight: 0.5 * cm;

        onStartTimeChanged: recalibrate();
        onEndTimeChanged: recalibrate();
        onPpsChanged: recalibrate();
        onViewportWidthChanged: recalibrate();

        model: traceModel;
        threadSliceDelegate: ThreadSlice {
            x: startTime * view.pps;
            width: (endTime - startTime) * view.pps;
        }
        rowBackgroundDelegate: RowGradient {
            width: view.pps * traceModel.traceLength
        }
        viewportRoot: contentRoot.contentItem

        Flickable {
            id: contentRoot;
            anchors.fill: parent;
            anchors.leftMargin: view.labelWidth;
            contentWidth: view.pps * traceModel.traceLength
            contentHeight: view.height // ### needs to be properly exposed from the model based on amount of rows..
            clip: true;
        }
    }

    ParallelAnimation {
        id: zoomAnimation
        NumberAnimation { id: ppsAnim; target: view; property: "pps"; duration: 150; easing.type: Easing.OutQuint }
        NumberAnimation { id: flickAnima; target: contentRoot; property: "contentX"; duration: 150; easing.type: Easing.OutQuint}
        function start(pps) {
            if (zoomAnimation.running)
                return;
            // Limit pps to the width of the trace.
            pps = Math.max(header.width / traceModel.traceLength, pps);
            var d = pps / view.pps;
            var contentMove = header.width / 2 * d;
            ppsAnim.to = pps;
            flickAnima.to = Math.max(0, (contentRoot.contentX + header.width / 2) * d - header.width / 2);
            zoomAnimation.running = true;
        }
    }

    NumberAnimation {
        id: moveAnimation
        target: contentRoot; property: "contentX"; duration: 150; easing.type: Easing.OutQuint;
        function start(delta) {
            if (moveAnimation.running)
                return;
            // bound the target so we don't go outside..
            to = Math.min(contentRoot.contentWidth - header.width, Math.max(0, contentRoot.contentX + delta));
            running = true;
        }
    }

    focus: true
    Keys.onPressed: {
        if (event.key == Qt.Key_Up || event.key == Qt.Key_W) {
            zoomAnimation.start(view.pps * 2.0);
        } else if (event.key == Qt.Key_Down || event.key == Qt.Key_S) {
            zoomAnimation.start(view.pps * 0.5);
        } else if (event.key == Qt.Key_Left || event.key == Qt.Key_A) {
            moveAnimation.start(-header.width * 0.33);
        } else if (event.key == Qt.Key_Right || event.key == Qt.Key_D) {
            moveAnimation.start(header.width * 0.33);
        }
    }

}
