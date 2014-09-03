import QtQuick 2.0

Rectangle {
    id: root

    width: 100
    height: 50

    property QtObject model;
    property string label;

    gradient: Gradient {
        GradientStop { position: 0; color: Qt.hsla(0, 0, 0.9); }
        GradientStop { position: 1 - 1 / root.height ; color: Qt.hsla(0, 0, 0.6); }
        GradientStop { position: 1; color: Qt.hsla(0, 0, 0.4); }
    }

    Repeater {
        id: repeater
        model: root.model

        Rectangle {

            property real t0: startSeconds * 1000 + startMicroSeconds / 1000;
            property real t1: endSeconds * 1000 + endMicroSeconds / 1000;

            Component.onCompleted: print(root.label + ": " + t0 + " -> " + t1 + " -- " + frequency)

            x: t0 * 0.5
            width: (t1 - t0) * 0.5

            property real hz: frequency / 1404000;

            y: root.height - hz * root.height;
            height: root.height * hz

            color: Qt.hsla((1 - hz) * 0.33, 1, 0.5, 0.66);

            antialiasing: true



//            Text {
//                color: "white"
//                text: frequency
//                font.pixelSize: 10
//                anchors.centerIn: parent
//                rotation: -90
//            }
        }
    }



}
