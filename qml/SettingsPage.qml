import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: root
    title: "Настройки"
    visible: false

    background: Rectangle {
        id: _backgroundRect
        color: "#f0f0f0"
    }


    RowLayout {
    anchors.horizontalCenter: parent.horizontalCenter

    Text{
        Layout.alignment: Qt.AlignVCenter
        text: 'Монитор:'
        font.pointSize: 12
    }

    ComboBox {
        id: _monitorListBox
        model: screenList
        textRole: "name"

        Component.onCompleted: currentIndex = model.getCurrentScreen()
        onActivated: {
            model.setCurrentScreen(_monitorListBox.currentIndex)
        }
    }

    }
}
