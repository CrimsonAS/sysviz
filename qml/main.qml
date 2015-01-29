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

import QtQuick 2.2
import SysViz 1.0

import "theme.js" as Theme;

Rectangle {
    id: root

    width: 800;
    height: 600;

    property var rowHeight: 1 * cm;

    gradient: Gradient {
        GradientStop { position: 0; color: Qt.hsla(0.66, 0.2, 0.4); }
        GradientStop { position: 1; color: Qt.hsla(0, 0, 0.2); }
    }

    Text {
        id: statusBar
        color: "white"
        font.pixelSize: 0.25 * cm;
        anchors.baseline: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 0.1 * cm;
        anchors.baselineOffset: - Math.floor(0.15 * cm)
    }

    TraceView {
        id: view;
        width: parent.width
        height: parent.height - statusBar.height - 0.2 * cm - header.height
        y: header.height

        onDisplayToolTip: {
            statusBar.text = text;
        }
        onHideToolTip: {
            statusBar.text = "";
        }
    }

    Item {
        width: view.viewportWidth
        x: view.width - view.viewportWidth
        height: parent.height
        clip: true;

        Header {
            id: header;

            x: 0
            width: parent.width
            height: Math.floor(0.5 * cm);

            pps: view.pps;
            startTime: view.startTime

            viewHeight: view.height
        }
    }




    focus: true
    Keys.onPressed: {
        if (event.key == Qt.Key_Up || event.key == Qt.Key_W) {
            view.startZoomAnimation(view.pps * 2.0);
        } else if (event.key == Qt.Key_Down || event.key == Qt.Key_S) {
            view.startZoomAnimation(view.pps * 0.5);
        } else if (event.key == Qt.Key_Left || event.key == Qt.Key_A) {
            view.startMoveAnimation(-header.width * 0.2);
        } else if (event.key == Qt.Key_Right || event.key == Qt.Key_D) {
            view.startMoveAnimation(header.width * 0.2);
        }
    }

}
