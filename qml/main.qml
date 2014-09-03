import QtQuick 2.0
import QtQuick.Window 2.0

Rectangle {
    id: root

    width: 800;
    height: 600;

    visible: true

    Column {

        Repeater {
            id: cpuFrequencyRepeater
            model: traceModel.cpuCount

            CpuFrequencyView {
                id: cpuFrequencyView
                model: traceModel.cpuFrequencyModel(index);
                width: root.width
                label: "CPU Frequency " + index;
            }

        }
    }

    gradient: Gradient {
        GradientStop { position: 0; color: Qt.hsla(0.66, 0.2, 0.3); }
        GradientStop { position: 1; color: Qt.hsla(0, 0, 0.1); }
    }
}
