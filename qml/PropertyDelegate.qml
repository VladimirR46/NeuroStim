import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs

Rectangle {
    id: root

    property var delegateComponentMap: {
        "edit": _editDelegate,
        "checkbox": _checkboxDelegate,
        "range": _rangeDelegate,
        "directorypath": _directoryPathDelegate,
        "colordialog": _colorDialogDelegate,
    }

    RowLayout {
        anchors.fill: parent

        Text {
            Layout.fillWidth: true
            text: model.desc
            //font.pointSize: 10
            color: "#353637"
        }
        Loader{
            Layout.alignment: Qt.AlignRight
            Layout.rightMargin: 10
            sourceComponent: delegateComponentMap[type]
        }
    }

    Rectangle {
        anchors.bottom: parent.bottom
        width: parent.width
        height: 1
        color: "#f0f0f0"
    }


    Component {
        id: _editDelegate

        TextField {
            width: 60
            height: 30
            text: model.value
            onEditingFinished: {
                model.value = text
            }
        }
    }

    Component{
        id: _checkboxDelegate

        CheckBox {
            checked: model.value
            onClicked: {
               model.value = checked
           }
        }
    }

    Component{
        id: _rangeDelegate

        Row {
            spacing: 6

            TextField {
                id: _value1
                width: 50
                height: 30
                text: model.value.x
                onEditingFinished: {
                    model.value = Qt.point(_value1.text, _value2.text)
                }
            }
            TextField {
                id: _value2
                width: 50
                height: 30
                text: model.value.y
                onEditingFinished: {
                    model.value = Qt.point(_value1.text, _value2.text)
                }
            }
        }
    }

    Component{
        id: _directoryPathDelegate

        Item{
            width: 50
        }
    }

    Component{
        id: _colorDialogDelegate

        Rectangle {
            height: 20
            width: 50
            color: model.value
            border.color: "black"
            radius: 20

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    _propertyView.currentIndex = index
                    _colorDialog.selectedColor = model.value
                    _colorDialog.open()
                }
            }
        }
    }
}
