import QtQuick 2.0

ThreadSliceBase {
    id: root

    function hashCode(str) {
        var hash = 0;
        for (var i = 0; i < str.length; i++) {
            hash = str.charCodeAt(i) + ((hash << 5) - hash);
        }
        return hash;
    }

    onLabelChanged: {
        var mod = 37
        var moded = Math.abs(hashCode(label) % mod) / mod;
        box.color = Qt.hsla(moded, 0.5, 0.5 - depth * 0.3);
    }

    Rectangle {
        id: box
        anchors.fill: parent
        anchors.bottomMargin: 1
    }

}
