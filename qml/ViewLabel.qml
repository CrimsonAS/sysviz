import QtQuick 2.0

Rectangle {

    property alias text: label.text

    gradient: Gradient {
        GradientStop { position: 0; color: Qt.hsla(0.66, 0.2, 0.4); }
        GradientStop { position: 1; color: Qt.hsla(0, 0, 0.2); }
    }

   Text {
       id: label
        anchors.centerIn: parent
        text: "Frequency:\nGPU"
        color: "white"
        horizontalAlignment: Text.AlignHCenter
        font.bold: true
        font.italic: true
    }
}
