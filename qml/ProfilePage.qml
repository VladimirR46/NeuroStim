import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: root
    title: "Профиль"

    background: Rectangle {
        id: _backgroundRect
        color: "#f0f0f0"
    }

    Image {
        id: _profileImage
        anchors.horizontalCenter: parent.horizontalCenter
        source: "qrc:/icons/icons8_applicant.svg"
    }

    Rectangle{
        anchors.top: _profileImage.bottom
        anchors.topMargin: 6
        anchors.horizontalCenter: parent.horizontalCenter
        width: 300
        height: 250
        //color: "lightgreen"
        radius: 15


        ColumnLayout {
            spacing: 10
            anchors.fill: parent
            anchors.leftMargin: 10
            //anchors.horizontalCenter: parent.horizontalCenter

            TextField{
                placeholderText: "Фамилия"
                Layout.fillWidth: true
                onEditingFinished:{
                    Subject.lastName = text
                }

                background: Rectangle {
                    height: 1
                    anchors.bottom: parent.bottom
                    color: "#f0f0f0"
                }
            }

            TextField{
                placeholderText: "Имя"
                Layout.fillWidth: true
                onEditingFinished:{
                    Subject.name = text
                }
                background: Rectangle {
                    height: 1
                    anchors.bottom: parent.bottom
                    color: "#f0f0f0"
                }
            }

            RowLayout {
                width: parent.width
                spacing: 10
                Text{
                    Layout.fillWidth: true
                    text: "Возраст"
                    color: "#353637"
                }
                SpinBox {
                    id: _spinBox
                    value: 18
                    Layout.rightMargin: 35
                    onValueModified: Subject.age = value
                }
            }

            ButtonGroup { id: genderGroup }
            ButtonGroup { id: handGroup }
            GridLayout {
                columns: 3

                Text{
                    Layout.fillWidth: true
                    text: "Пол:"
                    color: "#353637"
                }
                RadioButton {
                    text: "M"
                    onCanceled: Subject.gender = "male"
                    ButtonGroup.group: genderGroup
                }
                RadioButton {
                    //Layout.rightMargin: 40
                    text: "Ж"
                    onClicked: Subject.gender = "female"
                    ButtonGroup.group: genderGroup
                }

                Text{
                    Layout.fillWidth: true
                    text: "Рука:"
                    color: "#353637"
                }
                RadioButton {
                    text: "Левая"
                    onClicked: Subject.activehand = "left"
                    ButtonGroup.group: handGroup
                }
                RadioButton {
                    //Layout.rightMargin: 20
                    text: "Правая"
                    onClicked: Subject.activehand = "right"
                    ButtonGroup.group: handGroup
                }
            }
        }

    }

}
