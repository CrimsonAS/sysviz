import QtQuick 2.2

import "theme.js" as Theme;

Rectangle {
    id: root

    width: 800;
    height: 600;

    property var rowHeight: 1 * cm;

    Header {
        id: header;
    }

    Flickable {
        id: labels

        anchors.top: header.bottom
        width: 3 * cm // TODO: we need a way to fit this better perhaps.. and/or maybe offer a splitter to resize?
        height: flickable.height

        contentY: flickable.contentY

        clip: true

        Column {
            width: parent.width

            ViewLabel {
                height: root.rowHeight
                width: parent.width
                text: "Frequency:\nGPU"
            }

            Repeater {
                model: traceModel.cpuCount
                ViewLabel {
                    height: root.rowHeight
                    width: parent.width
                    text: "Frequency:\nCPU " + index;
                }
            }

            ViewLabel {
                height: root.rowHeight
                width: parent.width
                text: "IO Events"
            }

            Repeater {
                model: traceModel.threadCount
                ViewLabel {
                    property var tm: traceModel.threadModel(index)
                    height: Math.max(root.rowHeight, 10 * tm.maxStackDepth)
                    width: parent.width
                    text: "PID: " + tm.pid + "\n" + tm.threadName
                }
            }
        }
    }

    Flickable {
        id: flickable

        anchors.left: labels.right
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: header.bottom
        clip: true

//        NumberAnimation on contentX { from: 0; to: 500; duration: 2000; loops: Animation.Infinite }

        contentHeight: cm * cpuFrequencyRepeater.count;
        contentWidth: traceModel.traceLength * header.pps;

//        flickableDirection: Flickable.HorizontalFlick
//        draggingVertically: false
//        draggingHorizontally: true

        Column {
            FrequencyView {
                model: traceModel.gpuFrequencyModel();
                width: flickable.contentWidth;
                height: Math.floor(1 * cm);
                pps: header.pps;
                maxFrequency: traceModel.maxGpuFrequency
            }

            Repeater {
                id: cpuFrequencyRepeater
                model: traceModel.cpuCount
                FrequencyView {
                    model: traceModel.cpuFrequencyModel(index);
                    width: flickable.contentWidth;
                    height: Math.floor(1 * cm);
                    pps: header.pps;
                    maxFrequency: traceModel.maxCpuFrequency
                }
            }

            FrequencyView {
                model: traceModel.ioTrafficModel()
                width: flickable.contentWidth
                height: Math.floor(1 * cm);
                pps: header.pps
                maxFrequency: traceModel.maxIOTraffic
            }

            Repeater {
                id: threadRepeater
                model: traceModel.threadCount

                ThreadView {
                    model: traceModel.threadModel(index)
                    width: flickable.contentWidth
                    height: Math.max(root.rowHeight, 10 * model.maxStackDepth) // TODO: would be nice to sync these up with the headers so we didn't have to specify them twice
                    pps: header.pps
                }
            }
        }

    }

    gradient: Gradient {
        GradientStop { position: 0; color: Qt.hsla(0.66, 0.2, 0.4); }
        GradientStop { position: 1; color: Qt.hsla(0, 0, 0.2); }
    }


    ParallelAnimation {
        id: zoomAnimation
        NumberAnimation { id: ppsAnim; target: header; property: "pps"; duration: 150; easing.type: Easing.OutQuint }
        NumberAnimation { id: flickAnima; target: flickable; property: "contentX"; duration: 150; easing.type: Easing.OutQuint}
        function start(pps) {
            if (zoomAnimation.running)
                return;
            // Limit pps to the width of the trace.
            pps = Math.max(header.width / traceModel.traceLength, pps);
            var d = pps / header.pps;
            var contentMove = header.width / 2 * d;
            ppsAnim.to = pps;
            flickAnima.to = Math.max(0, (flickable.contentX + header.width / 2) * d - header.width / 2);
            zoomAnimation.running = true;
        }
    }

    NumberAnimation {
        id: moveAnimation
        target: flickable; property: "contentX"; duration: 150; easing.type: Easing.OutQuint;
        function start(delta) {
            if (moveAnimation.running)
                return;
            // bound the target so we don't go outside..
            to = Math.min(flickable.contentWidth - header.width, Math.max(0, flickable.contentX + delta));
            running = true;
        }
    }

    focus: true
    Keys.onPressed: {
        if (event.key == Qt.Key_Up || event.key == Qt.Key_W) {
            zoomAnimation.start(header.pps * 2.0);
        } else if (event.key == Qt.Key_Down || event.key == Qt.Key_S) {
            zoomAnimation.start(header.pps * 0.5);
        } else if (event.key == Qt.Key_Left || event.key == Qt.Key_A) {
            moveAnimation.start(-header.width * 0.33);
        } else if (event.key == Qt.Key_Right || event.key == Qt.Key_D) {
            moveAnimation.start(header.width * 0.33);
        }
    }


}
