import QtQuick 2.0
import QtQuick.Window 2.0
import SysViz 1.0

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

            Row {
                Repeater {
                    model: tm.cpuCount

                    Column {
                        Text {
                            font.pixelSize: 30
                            text: "CPU ID " + modelData + " (frequency)"
                        }
                        Repeater {
                            model: tm.cpuFrequencyModel(modelData)
                            delegate: Text {
                                text: model.startSeconds + "." + model.startMicroSeconds + "-" + model.endSeconds + "." + model.endMicroSeconds + " -- " + model.frequency
                            }
                        }
                    }
                }
                Repeater {
                    model: tm.cpuCount

                    Column {
                        Text {
                            font.pixelSize: 30
                            text: "CPU ID " + modelData + " (C-state)"
                        }
                        Repeater {
                            model: tm.cpuCStateModel(modelData)
                            delegate: Text {
                                text: model.startSeconds + "." + model.startMicroSeconds + "-" + model.endSeconds + "." + model.endMicroSeconds + " -- " + model.cstate

                                color: {
                                    switch (model.cstate) {
                                        case 0:
                                            return "#aa3939"
                                        case 1:
                                            return "#aa6c39"
                                        case 2:
                                            return "#2d882d"
                                        case 3:
                                            return "#226666"
                                    }
                                }
                            }
                        }
                    }
                }

                Column {
                    Text {
                        font.pixelSize: 30
                        text: "GPU frequency"
                    }
                    Repeater {
                        model: tm.gpuFrequencyModel()
                        delegate: Text {
                        text: model.startSeconds + "." + model.startMicroSeconds + "-" + model.endSeconds + "." + model.endMicroSeconds + " -- " + model.frequency
                        }
                    }
                }

                Column {
                    Text {
                        font.pixelSize: 30
                        text: "Processes & Threads"
                    }
                    Repeater {
                        id: outer
                        model: tm.processModel()
                        delegate: Column {
                            id: delegate
                            property var mytm: model.threadModel
                            Text {
                                text: model.pid + " " + model.threadName
                            }

                            Repeater {
                                model: delegate.mytm
                                delegate: Text{
                                    //text: model.name
                                }
                            }
                        }
                    }
                }

            }
        }
    }
}
