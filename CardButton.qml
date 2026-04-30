import QtQuick 2.12

Rectangle {
    id: cardButton

    property alias text: label.text
    property color baseColor: "#404040"
    property color hoverColor: "#505050"
    property color pressedColor: "#4a4a4a"
    property color borderColor: "#4A90E2"
    property int fontSize: 26
    property bool enabled: true

    signal clicked()

    width: 150
    height: 75
    radius: 15
    color: enabled ? baseColor : "#2f2f2f"
    border.color: borderColor
    border.width: 2
    opacity: enabled ? 1.0 : 0.5

    Text {
        id: label
        anchors.centerIn: parent
        font.pixelSize: cardButton.fontSize
        color: "white"
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        enabled: cardButton.enabled

        onEntered: cardButton.color = cardButton.hoverColor
        onExited: cardButton.color = cardButton.baseColor
        onPressed: cardButton.color = cardButton.pressedColor
        onReleased: {
            cardButton.color = containsMouse ? cardButton.hoverColor : cardButton.baseColor
            if (containsMouse)
                cardButton.clicked()
        }
    }
}
