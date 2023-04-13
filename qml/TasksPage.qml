import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Tasks 1.0

Page {
    id: root
    title: "Эксперименты"
    visible: false

    background: Rectangle {
        id: _backgroundRect
        color: "#f0f0f0"
    }

    Image {
        id: _taskLogo
        anchors.horizontalCenter: parent.horizontalCenter
        source: "qrc:/icons/icons8_system_task.svg"
        sourceSize.height: 80
        sourceSize.width: 80
    }

    Rectangle {
        id: _lvRect
        anchors.top: _taskLogo.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: 350
        height: 400
        color: "white"
        radius: 15


        ListView {
            id: _tasksListView
            anchors.fill: parent
            anchors.margins: 10
            clip: true
            spacing: 8

            model:TaskListModel{
                manager: taskManager
            }

            delegate: Rectangle {
                id: _delegate
                width: _tasksListView.width
                height: 60
                radius: 10

                Image {
                    id: _taskImage
                    anchors.left: parent.left
                    source: "qrc:/icons/taskIcon.svg"
                }

                Column {
                    anchors.left: _taskImage.right
                    anchors.right: _delegate.right
                    Text {
                        text: name
                        font.bold: true
                        font.pointSize: 12
                    }
                    Text{
                        width: parent.width
                        text: desc
                        font.pointSize: 10
                        wrapMode: Text.Wrap
                        color: "gray"
                    }
                }

                Rectangle {
                    width: _delegate.width
                    height: 1
                    anchors.bottom: parent.bottom
                    color: "#f0f0f0"
                }

                MouseArea{
                    anchors.fill: parent
                    hoverEnabled: true

                    onClicked: {
                        _tasksListView.currentIndex = index
                        _taskDialog.properties = taskManager.taskProperties(index)
                        _taskDialog.title = name
                        _taskDialog.open()
                    }
                    onEntered: _delegate.color = "#f0f0f0"
                    onExited: _delegate.color = "white"
                }
            }
            ScrollIndicator.vertical: ScrollIndicator { }
        }
    }

    TaskDialog {
        id: _taskDialog
    }

}
