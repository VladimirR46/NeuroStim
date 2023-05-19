import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs
import Mail 1.0

Dialog {
    id: root
    x: (parent.width - width) / 2
    y: 20
    parent: Overlay.overlay
    focus: true
    modal: true
    title: "Отправить"
    width: 400
    height: 300

    function mail_body(index){
        var model = _historyListView.model
        var subject = model.data(model.index(index,0), Qt.UserRole+2)
        var datetime = model.data(model.index(index,0), Qt.UserRole+3)

        return 'Испытуемый: '+subject+'\nНачало эксперимента: '+datetime;
    }

    GridLayout{
        columns: 2
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: _sendButton.top
        anchors.top: parent.top
        anchors.bottomMargin: 10
        Text{
            text: "Почта:"
            font.pointSize: 12
        }
        TextField{
            id: _mailField
            Layout.fillWidth: true
            placeholderText: "example@mail.ru"
        }

        Text{
            Layout.alignment: Qt.AlignTop
            text: "Текст:"
            font.pointSize: 12
        }

        Rectangle {
            border.color: "#bdbdbd"
            border.width: 1
            Layout.fillWidth: true
            Layout.fillHeight: true

            TextArea {
                id: _textArea
                anchors.fill: parent
                anchors.margins: 1
                wrapMode: TextEdit.WrapAnywhere
                text: mail_body(_historyListView.currentIndex)
            }
        }
    }


    Button{
        id: _sendButton
        anchors.right: parent.right
        //anchors.rightMargin: 10
        anchors.bottom: parent.bottom
        icon.color: "transparent"
        icon.source: "qrc:/icons/send.svg"
        icon.height: 24
        icon.width: 24
        text: "Отправить"
        /*background: Rectangle{
            color: _sendButton.down ? "skyblue" : (_sendButton.hovered ? "#f0f0f0" : "white")
            radius: 10
            border.color: "powderblue"
            border.width: 1
        }*/
        onClicked: {
            _mailAgent.sendMail(_mailField.text, _textArea.text, _historyListView.model.getExperiment(_historyListView.currentIndex))
            root.close()
        }
    }

    MailAgent{
        id: _mailAgent
    }

}
