/*
 * Copyright (C) 2014 Gunnar Sletta <gunnar@sletta.org>
 * Copyright (C) 2014 Robin Burchell <robin.burchell@viroteck.net>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

import QtQuick 2.0

import "theme.js" as Theme;

RowGradient {
    id: root

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

    property real viewHeight;


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
            Rectangle {
                anchors.top: parent.bottom
                height: root.viewHeight
                width: 1
                color: Qt.hsla(0, 0, 0, 0.1);
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
