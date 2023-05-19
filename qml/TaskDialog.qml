import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Properties 1.0
import QtQuick.Dialogs

Dialog {
    id: root
    x: (parent.width - width) / 2
    y: 10
    parent: Overlay.overlay
    focus: true
    modal: true
    //title: "Настройки"
    width: 400
    height: 500

    ColorDialog {
        id: _colorDialog
        title: "Please choose a color"
        onAccepted: {
            var model = _propertyView.model
            model.setData(model.index(_propertyView.currentIndex,0), selectedColor , Qt.UserRole+3) // Role - color =
        }
    }

    background: Rectangle{
        color: "#f0f0f0"
    }

    property alias properties: _propertyListModel.properties

    ColumnLayout {
        anchors.fill: parent
        spacing: 8

        Image {
            id: _logo
            Layout.alignment: Qt.AlignHCenter
            source: "qrc:/icons/icons8_choice.svg"
            sourceSize.height: 80
            sourceSize.width: 80
        }

        Rectangle {
            Layout.fillHeight: true
            width: root.width - 30
            //height: 100
            radius: 10

            ListView {
                id: _propertyView
                anchors.margins: 10
                anchors.fill: parent

                clip: true
                model: PropertyListModel {
                    id: _propertyListModel
                }

                delegate: PropertyDelegate {
                    width: _propertyView.width
                    height: 40
                }
                ScrollIndicator.vertical: ScrollIndicator { }
            }
        }


        Row{
            spacing: 8
            //anchors.horizontalCenter: parent.horizontalCenter
            Layout.fillWidth: true

            Button{
                id: _previewButton
                text: "Просмотр"
                icon.color: "transparent"
                icon.source: "qrc:/icons/preview.svg"
                icon.height: 30
                icon.width: 30
                background: Rectangle{
                    color: _previewButton.down ? "skyblue" : (_previewButton.hovered ? "#f0f0f0" : "white")
                    radius: 10
                    border.color: "powderblue"
                    border.width: 1
                }

                onClicked: {
                    appEngine.startPreviewTask(_tasksListView.currentIndex)
                    root.close()
                }
            }

            Button{
                id: _startButton
                text: "Запустить"
                icon.color: "transparent"
                icon.source: "qrc:/icons/play.svg"
                icon.height: 32
                icon.width: 32
                background: Rectangle{
                    color: _startButton.down ? "skyblue" : (_startButton.hovered ? "#f0f0f0" : "white")
                    radius: 10
                    border.color: "powderblue"
                    border.width: 1
                }
                onClicked: {
                    appEngine.startSelectedTask(_tasksListView.currentIndex)
                    root.close()
                }
            }
        }

    }

}
