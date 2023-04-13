import QtQuick
import QtQuick.Layouts

Item {
    id: root

    property alias text: _text.text
    property alias source: _icon.source

    signal clicked()

    Rectangle {
        anchors.fill: parent
        color: _mouseArea.containsPress  ? "skyblue" : (_mouseArea.containsMouse ? "#f0f0f0" : "white")


        Row {
            anchors.fill: parent
            anchors.leftMargin: 10
            spacing: 10

            Image {
                id: _icon
                anchors.verticalCenter: parent.verticalCenter
                sourceSize.height: 32
                sourceSize.width: 32
            }

            Text {
                id: _text
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("Button")
                color: "#353637"
            }
        }

        MouseArea {
            id: _mouseArea
            hoverEnabled: true
            anchors.fill: parent

            onClicked: root.clicked()
        }
    }

}
