import QtQuick 2.0
import QtQuick.Window 2.0

import "theme.js" as Theme;

Rectangle {
    id: root

    width: 800;
    height: 600;

    visible: true

    RowGradient {
        id: header

        width: parent.width

        height: Math.floor(0.5 * cm);

        // Pixels per millisecond, a kinda of zoom ratio
        property real pps: 100;

//        NumberAnimation on pps { from: 100; to: 1000; duration: 10000; loops: Animation.Infinite }

        property real startTime: flickable.contentX / pps;
        property real visibleTimeSpan: width / pps;
        property int primaryUnitCount: Math.ceil(visibleTimeSpan) + 1;
        property real primaryUnitOffset: fmod(startTime, 1);

        function fmod(x, y) {
            return x - Math.floor(x / y);
        }

        Repeater {
            id: primaryUnitRepeater
            model: header.primaryUnitCount
            Rectangle {
                width: 1
                color: Theme.colors.timeIndicatorPrimary;
                height: header.height * 0.33
                y: header.height - height - 1;
                x: (-header.primaryUnitOffset + index) * header.pps;
                antialiasing: true
                Text {
                    anchors.bottom: parent.top
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: (Math.floor(header.startTime) + index) + " s";
                    font.pixelSize: header.height * 0.4;
                }
            }
        }

    }

    Flickable {
        id: flickable

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: header.bottom

//        NumberAnimation on contentX { from: 0; to: 500; duration: 2000; loops: Animation.Infinite }

        contentHeight: cm * cpuFrequencyRepeater.count;
        contentWidth: traceModel.traceLength * header.pps;

        flickableDirection: Flickable.HorizontalFlick
//        draggingVertically: false
//        draggingHorizontally: true

        Column {
//            height: 2 * cm
            Repeater {
                id: cpuFrequencyRepeater
                model: traceModel.cpuCount
                CpuFrequencyView {
                    id: cpuFrequencyView
                    model: traceModel.cpuFrequencyModel(index);
                    width: 10000
                    label: "CPU Frequency " + index;
                    height: Math.floor(1 * cm);
                    pps: header.pps;
                }
            }
        }

    }


    gradient: Gradient {
        GradientStop { position: 0; color: Qt.hsla(0.66, 0.2, 0.9); }
        GradientStop { position: 1; color: Qt.hsla(0, 0, 0.5); }
    }

}
