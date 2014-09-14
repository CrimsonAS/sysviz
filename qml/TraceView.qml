import QtQuick 2.3
import SysViz 1.0
import "theme.js" as Theme;

Item {
    id: root

    property real labelWidth: 3 * cm
    property real pps: 10 * cm;
    property real viewportWidth: width - labelWidth;
    property real contentWidth: pps * traceModel.traceLength

    property real rowSpacing: 1;

    property real startTime: contentFlickable.contentX / pps;
    property real endTime: startTime + contentFlickable.width / pps;

    property real threadSliceHeight: 0.5 * cm;

    property Item contentRoot: contentFlickable.contentItem

    signal displayToolTip(string text)
    signal hideToolTip()

    Flickable {
        id: contentFlickable;
        anchors.fill: parent;
        anchors.leftMargin: root.labelWidth;
        contentWidth: root.contentWidth
        contentHeight: threadSlicesColumn.height + 2 * Theme.sizes.columnSpacing * cm
        clip: true;

        Column {
            id: threadSlicesColumn
            width: root.contentWidth
            y: Theme.sizes.columnSpacing * cm
            spacing: Theme.sizes.columnSpacing * cm;

            FrequencyView {
                width: root.contentWidth
                height: Theme.sizes.frequencyHeight * cm
                model: traceModel.gpuFrequencyModel()
                pps: root.pps;
                max: traceModel.maxGpuFrequency
                color: Qt.hsla(0.33, 0.8, 0.3, 0.25);
            }

            Repeater {
                id: cpuFrequencyRepeater
                model: traceModel.cpuCount
                FrequencyView {
                    width: root.contentWidth
                    height: Theme.sizes.frequencyHeight * cm
                    model: traceModel.cpuFrequencyModel(index)
                    pps: root.pps
                    max: traceModel.maxCpuFrequency
                    color: Qt.hsla(0, 0.8, 0.3, 0.25);
                }
            }

            Repeater {
                id: threadSliceRepeater
                model: traceModel.threadCount

                RowGradient {
                    id: threadSliceViewRoot
                    width: root.contentWidth
                    height: sliceView.height + 2 * Theme.sizes.columnPadding * cm

                    ThreadSliceView {
                        id: threadSliceBg;
                        y: Theme.sizes.columnPadding * cm
                        model: traceModel.threadModel(index);
                        width: parent.width
                        startTime: 0;
                        endTime: traceModel.traceLength
                        transform: Scale {
                            xScale: root.pps
                        }

                        delegate: ThreadSliceBox {
                            x: startTime
                            height: Theme.sizes.sliceHeight * cm;
                            width: (endTime - startTime);
                        }

                        onHoveringOver: {
                            if (item == null)
                                root.hideToolTip();
                            else
                                root.displayToolTip(item.label + "; length="
                                                    + Math.round((item.endTime - item.startTime) * 100000) / 100
                                                    + " ms; " + "start=" + item.startTime);
                        }
                    }


                    ThreadSliceView {
                        id: sliceView;
                        y: Theme.sizes.columnPadding * cm
                        model: traceModel.threadModel(index);
                        width: parent.width
                        startTime: visible ? root.startTime : 0
                        endTime: visible ? root.endTime : 0
                        minimumSliceLength: 2 * cm / root.pps;
                        // Set enabled to false so the other threaSliceBg gets the hover events
                        enabled: false
                        delegate: ThreadSliceLabel {
                            x: startTime * root.pps
                            height: Theme.sizes.sliceHeight * cm;
                            width: (endTime - startTime) * root.pps;
                        }

                        Connections {
                            target: contentFlickable
                            onContentXChanged: sliceView.updateVisibility();
                            onHeightChanged: sliceView.updateVisibility();
                        }

                        function updateVisibility() {
                            var yStart = mapToItem(contentFlickable, 0, 0).y
                            var yEnd = mapToItem(contentFlickable, 0, height).y
                            visible = yEnd > 0 && yStart < contentFlickable.height;
                        }
                        onVisibleChanged: calibrate();
                    }
                }
            }
        }

    }

    Item {
        id: labelRootParent
        width: root.labelWidth
        height: parent.height
        clip: true
        Column {
            id: labelContentRoot;
            y: -contentFlickable.contentY + Theme.sizes.columnSpacing * cm
            width: parent.width
            height: contentFlickable.contentHeight
            spacing: Theme.sizes.columnSpacing * cm

            ViewLabel {
                width: parent.width
                height: Theme.sizes.frequencyHeight * cm
                text: "GPU Frequency: "
            }

            Repeater {
                model: traceModel.cpuCount
                ViewLabel {
                    width: parent.width
                    height: Theme.sizes.frequencyHeight * cm
                    text: "CPU Frequency: " + index;
                }
            }

            Repeater {
                model: traceModel.threadCount
                ViewLabel {
                    property var model: traceModel.threadModel(index);
                    width: parent.width
                    height: model.maxStackDepth * Theme.sizes.sliceHeight * cm + 2 * Theme.sizes.columnPadding * cm
                    text: "PID: " + model.pid + "\n" + model.threadName;
                }
            }
        }
    }

    ParallelAnimation {
        id: zoomAnimation
        NumberAnimation { id: ppsAnim; target: view; property: "pps"; duration: 150; easing.type: Easing.OutQuint }
        NumberAnimation { id: flickAnima; target: contentFlickable; property: "contentX"; duration: 150; easing.type: Easing.OutQuint}
    }

    function startZoomAnimation(pps) {
        if (zoomAnimation.running)
            return;
        // Limit pps to the width of the trace.
        pps = Math.max(root.width / traceModel.traceLength, pps);
        var d = pps / root.pps;
        var contentMove = root.width / 2 * d;
        ppsAnim.to = pps;
        flickAnima.to = Math.max(0, (contentFlickable.contentX + root.width / 2) * d - root.width / 2);
        zoomAnimation.running = true;
    }


    NumberAnimation {
        id: moveAnimation
        target: contentFlickable; property: "contentX"; duration: 150; easing.type: Easing.OutQuint;
    }
    function startMoveAnimation(delta) {
        if (moveAnimation.running)
            return;
        // bound the target so we don't go outside..
        moveAnimation.to = Math.min(contentFlickable.contentWidth - root.width, Math.max(0, contentFlickable.contentX + delta));
        moveAnimation.running = true;
    }


}
