import QtQuick 2.0
import QtQuick.Window 2.0
import my.thing 1.0

Window {
    height: 200
    width: 200
    visible: true
    title: "sysviz"

    TraceModel {
        id: tm
    }

    Flickable {
        anchors.fill: parent
        contentHeight: col.childrenRect.height
        Row {
            id: col
            Repeater {
                model: tm.cpuCount

                Column {
                    Text {
                        font.pixelSize: 30
                        text: "CPU ID " + modelData
                    }
                    Repeater {
                        model: tm.cpuFrequencyModel(modelData)
                        delegate: Text {
                            text: model.startSeconds + "." + model.startMicroSeconds + "-" + model.endSeconds + "." + model.endMicroSeconds + " -- " + model.frequency
                        }
                    }
                }
            }
        }
    }
}
