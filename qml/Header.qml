import QtQuick 2.0

import "theme.js" as Theme;

RowGradient {
    id: root

    clip: true

    property real pps: 20 * cm;         // pixels per second
    property real unit: 1;              // Unit scale factor of 10^x, where x is an integer.
    property real ppu: pps * unit;      // Pixels per unit

    property real startTime: 0;

    property real pixelsToStartTime: startTime * pps;
    property int fUnitIndex: Math.floor(pixelsToStartTime / ppu);
    property real fUnitPixels: fUnitIndex * ppu;
    property real fUnitOffset: fUnitPixels - pixelsToStartTime;
    property real fUnitTime: fUnitIndex * unit;

    property int unitCount: Math.ceil(width / ppu) + 1;

    property bool alternateLabelsOnly : false;


    Component.onCompleted: updateUnitScale();
    onPpsChanged: updateUnitScale()

//        function dumpVariables() {
//            print("pps: " + pps);
//            print("unit: " + unit);
//            print("ppu: " + ppu);
//            print("startTime: " + startTime);
//            print("pixelsToStartTime: " + pixelsToStartTime);
//            print("fUnitIndex: " + fUnitIndex);
//            print("fUnitPixels: " + fUnitPixels);
//            print("fUnitOffset: " + fUnitOffset);
//            print("fUnitTime: " + fUnitTime);
//            print("unitCount: " + unitCount);
//        }

    function updateUnitScale() {
        var distanceBetweenLabels = pps / cm;
        var scale = 1;
        while (distanceBetweenLabels * scale <= 2)
            scale *= 10;
        while (distanceBetweenLabels * scale >= 6)
            scale *= 0.1;
        alternateLabelsOnly = distanceBetweenLabels * scale < 2;
        unit = scale;
    }

    Repeater {
        id: primaryUnitRepeater
        model: root.unitCount
        Rectangle {
            property bool labelled: root.alternateLabelsOnly ? ((index + root.fUnitIndex) % 2) == 0 : true;
            width: labelled ? 2.0 : 1
            color: Theme.colors.timeIndicatorPrimary;
            height: root.height * 0.33
            y: root.height - height - 1;
            x: root.fUnitOffset + root.ppu * index;
            antialiasing: true
            Text {
                visible: parent.labelled;
                property real time: root.fUnitTime + index * root.unit;
                anchors.bottom: parent.top
                anchors.bottomMargin: 2
                anchors.horizontalCenter: parent.horizontalCenter
                text: root.unit < 1
                      ? root.unit < 0.001
                        ? Math.round(time * 1000000) + " us"
                        : Math.round(time * 1000) + " ms"
                      : Math.round(time) + " s";
                font.pixelSize: root.height * 0.4;
            }

            Repeater {
                model: 9
                Rectangle {
                    x: root.ppu * 0.1 * (1 + index)
                    width: 1
                    anchors.bottom: parent.bottom
                    height: parent.height * 0.66
                    color: Theme.colors.timeIndicatorSecondary
                    antialiasing: true
                }
            }
        }
    }
}
