import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window


Page {
    id:root

    header: ToolBar {
        height: 50
        leftPadding: 0

        background: Rectangle {
            anchors.fill: parent
            color : "#f0f0f0"
        }

        ToolButton{
            id: _menuButton
            height: parent.height
            width: 50
            anchors.verticalCenter: parent.verticalCenter
            icon.source: "qrc:/icons/menu.png"
            icon.width: 20
            icon.height: 20
            background: Rectangle {
                anchors.fill: parent
                color : _menuButton.pressed ? "skyblue" : "#f0f0f0"
            }
            onClicked: drawer.open()
        }

        Text{
            text: _stackView.currentItem.title
            font.pointSize: 16
            anchors.centerIn: parent
        }

    }

    Drawer {
        id: drawer
        width: 170
        height: root.height

        ColumnLayout{
            spacing: 0
            anchors.fill: parent
            anchors.topMargin: 10

            Image {
                source: "qrc:/icons/main44.png"
                Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            }

            Text{
                text: "NeuroStim"
                font.bold: true
                font.pointSize: 12
                Layout.alignment: Qt.AlignHCenter
            }
            Item{height: 10}
            Rectangle{
                width: parent.width
                height: 1
                color: "#f0f0f0"
            }


            MenuButton {
                Layout.fillWidth: true
                height: 40
                text: "Профиль"
                source: "qrc:/icons/profile.svg"
                onClicked: {
                    _stackView.pop()
                    drawer.close()
                }
            }
            MenuButton {
                Layout.fillWidth: true
                height: 40
                text: "Эксперименты"
                source: "qrc:/icons/tasks.svg"
                onClicked: {
                    _stackView.pop()
                    _stackView.push("qrc:/qml/TasksPage.qml")
                    drawer.close()
                }
            }
            MenuButton {
                Layout.fillWidth: true
                height: 40
                text: "Настройки"
                source: "qrc:/icons/settings.svg"
                onClicked: {
                    _stackView.pop()
                    _stackView.push("qrc:/qml/SettingsPage.qml")
                    drawer.close()
                }
            }
            Item{
                Layout.fillHeight: true
            }
            MenuButton {
                Layout.fillWidth: true
                height: 40
                text: "Выход"
                source: "qrc:/icons/exit.svg"
                onClicked: Qt.quit()
            }
        }
    }

    StackView {
        id: _stackView
        anchors.fill: parent
        initialItem: ProfilePage {
            id: _profilePage
        }
    }

}
