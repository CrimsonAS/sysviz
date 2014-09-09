import QtQuick 2.0

Rectangle {

    property alias text: label.text

    width: label.width + 0.5 * cm

    gradient: Gradient {
        GradientStop { position: 0; color: Qt.hsla(0.66, 0.1, 0.3); }
        GradientStop { position: 1; color: Qt.hsla(0.66, 0.1, 0.2); }
    }

   Text {
       id: label
        color: "white"
        x: 0.25 * cm
        y: 0.25 * cm
        font.bold: true
        font.italic: true
    }
}
