import QtQuick 2.12
import QtQuick.Controls 2.12

Popup {
    id: root

    property string titleText: "Dialog"
    property string messageText: ""
    property string confirmText: "OK"
    property string cancelText: "Cancel"
    property bool showConfirm: true
    property bool showCancel: false
    property color accentColor: "white"

    signal accepted()
    signal rejected()

    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape

    width: 700
    height: 350
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    padding: 0

    background: Rectangle {
        color: "#303030"
        radius: 10
        border.color: root.accentColor
        border.width: 2
    }

    Rectangle {
        id: headerRect
        width: parent.width
        height: 60
        color: "#303030"
        border.color: root.accentColor

        Text {
            anchors.centerIn: parent
            text: root.titleText
            color: "white"
            font.pixelSize: 30
            font.bold: true
        }
    }

    Item {
        id: body
        anchors.top: headerRect.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: footerRect.top

        Text {
            anchors.fill: parent
            anchors.margins: 20
            text: root.messageText
            color: "white"
            wrapMode: Text.WordWrap
            font.pixelSize: 28
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
    }

    Rectangle {
        id: footerRect
        width: parent.width
        height: 90
        anchors.bottom: parent.bottom
        border.color: root.accentColor
        color: "#303030"
        visible: root.showConfirm || root.showCancel

        Row {
            anchors.centerIn: parent
            spacing: 20

            CardButton {
                visible: root.showConfirm
                width: 180
                height: 70
                radius: 14
                fontSize: 24
                text: root.confirmText
                borderColor: root.accentColor

                onClicked: {
                    root.close()
                    root.accepted()
                }
            }

            CardButton {
                visible: root.showCancel
                width: 180
                height: 70
                radius: 14
                fontSize: 24
                text: root.cancelText
                borderColor: root.accentColor

                onClicked: {
                    root.close()
                    root.rejected()
                }
            }
        }
    }
}
