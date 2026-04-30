import QtQuick 2.12
import QtQuick.Controls 2.12

RadioButton {
    id: root

    // -------- Customizable props --------
    property color borderColor: "#4A90E2"
    property color fillColor: "#4A90E2"
    property color dotColor: "white"
    property color textColor: "white"
    property int textSize: 24

    // -------- Indicator --------
    indicator: Rectangle {
        width: 24
        height: 24
        radius: 12
        border.color: root.borderColor
        border.width: 2
        color: "transparent"
        anchors.verticalCenter: parent.verticalCenter

        Rectangle {
            anchors.centerIn: parent
            width: 10
            height: 10
            radius: 5
            color: root.dotColor
            visible: root.checked
        }
    }

    // -------- Text --------
    contentItem: Text {
        text: root.text
        color: root.textColor
        font.pixelSize: root.textSize
        verticalAlignment: Text.AlignVCenter

        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 40
    }
}
