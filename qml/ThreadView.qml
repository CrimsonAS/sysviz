import QtQuick 2.0

RowGradient {
    id: root

    width: 100
    height: 50

    property QtObject model;

    property real padding: 2
    property real pps: 1000;

    Repeater {
        id: repeater
        model: root.model

        Rectangle {

            radius: 3

            x: startTime * root.pps
            width: (endTime - startTime) * root.pps

            y: (root.height - padding) - height
            height: root.height - padding * 2

            color: Qt.hsla((1) * 0.33, 1, 0.5, 0.5);
            antialiasing: true
        }
    }



}
