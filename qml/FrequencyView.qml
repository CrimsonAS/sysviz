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
