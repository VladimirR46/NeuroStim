import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import History 1.0

Page {
    id: root
    title: "История"
    visible: false

    background: Rectangle {
        id: _backgroundRect
        color: "#f0f0f0"
    }

    MessageDialog {
        id: _deleteDialog
        text: qsTr("Удаление")

        informativeText: qsTr("Вы уверены что хотите удалить эксперимент?")
        buttons: MessageDialog.Ok | MessageDialog.Cancel
            onButtonClicked: function (button, role) {
                switch (button) {
                case MessageDialog.Ok:
                    _historyListView.model.removeExperiment(_historyListView.currentIndex)
                    _deleteDialog.close()
                    break;
                case MessageDialog.Cancel:
                    _deleteDialog.close()
                    break;
                }
            }
    }

    ListView {
        id: _historyListView
        anchors.fill: parent
        anchors.margins: 10
        clip: true
        spacing: 8

        model:HistoryListModel{

        }

        delegate: Rectangle {
            id: _delegate
            width: _historyListView.width
            height: 50
            radius: 10

            CheckBox {
                id: _checkBox
                anchors.verticalCenter: parent.verticalCenter
                onClicked: {

               }
            }

            Column {
                anchors.left: _checkBox.right
                spacing: 5
                Text {
                    text: exprtiment
                    font.bold: true
                    color: "#353637"
                }
                Text {
                    text: subject
                    color: "#353637"
                }
            }

            Text {
                anchors.right: _sendButton.left
                anchors.bottom: parent.bottom
                anchors.rightMargin: 15
                text: datatime
                color: "#6e6e6e"
            }

            Button{
                id: _sendButton
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: _deleteButton.left
                anchors.rightMargin: 15
                icon.color: "transparent"
                icon.source: "qrc:/icons/send.svg"
                icon.height: 24
                icon.width: 24
                background: Rectangle{
                    color: _sendButton.down ? "skyblue" : (_sendButton.hovered ? "#f0f0f0" : "white")
                    radius: 10
                    border.color: "powderblue"
                    border.width: 1
                }
                onClicked: {
                    _historyListView.currentIndex = index
                    _sendDialog.open()
                }
            }

            Button{
                id: _deleteButton
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 15
                icon.color: "transparent"
                icon.source: "qrc:/icons/delete_trash.svg"
                icon.height: 24
                icon.width: 24
                background: Rectangle{
                    color: _deleteButton.down ? "skyblue" : (_deleteButton.hovered ? "#f0f0f0" : "white")
                    radius: 10
                    border.color: "powderblue"
                    border.width: 1
                }
                onClicked: {
                    _historyListView.currentIndex = index
                    _deleteDialog.open()
                }
            }
        }
        ScrollIndicator.vertical: ScrollIndicator { }
    }

    SendDialog{
        id: _sendDialog
    }

}
