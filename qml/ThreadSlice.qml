import QtQuick 2.0

Item {
    id: root;

    property real depth;
    property real startTime;
    property real endTime;
    property string label;

    property Text labelItem;

    Rectangle {
        anchors.fill: parent
        anchors.bottomMargin: 1
        color: Qt.hsla(0.6, 0.5, 0.4 + 0.4 * depth);
    }

    onWidthChanged: showOrHideLabel()

    function showOrHideLabel() {
        if (width > 0.5 * cm) {
            if (labelItem == undefined)
                labelItem = textComponent.createObject(root, { text: root.label } )
        } else if (labelItem != undefined) {
            labelItem.destroy();
            labelItem = null;
        }
    }

    Component {
        id: textComponent
        Text {
            id: text;
            font.pixelSize: 0.2 * cm;
            color: "white"
            elide: Text.ElideRight
            visible: parent.width > 1.0 * cm;
            anchors.fill: parent
            anchors.leftMargin: 0.1 * cm
            anchors.rightMargin: 0.1 * cm
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
        }
    }
}
