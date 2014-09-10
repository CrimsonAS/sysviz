import QtQuick 2.3
import SysViz 1.0
import "theme.js" as Theme

RowGradient {
    id: root

    property alias graphType: graphItem.graphType
    property alias color: graphItem.color
    property alias model: graphItem.model
    property real pps;
    property real max;

    GraphItem {
        id: graphItem
        model: traceModel.cpuFrequencyModel(index);
        y: Theme.sizes.columnPadding * cm
        height: parent.height - Theme.sizes.columnPadding * 2 * cm
        transform:
            Matrix4x4 {
                property real sx: root.pps;
                property real sy: graphItem.height / root.max
                matrix: Qt.matrix4x4(sx, 0, 0, 0,
                                     0, -sy, 0, graphItem.height,
                                     0, 0, 1, 0,
                                     0, 0, 0, 1);
            }
        graphType: GraphItem.BarGraph
    }
}
