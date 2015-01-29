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

ThreadSliceBase {
    id: root

    property Text labelItem;

    onWidthChanged: showOrHideLabel()

    function showOrHideLabel() {
        if (width > 2 * cm) {
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
