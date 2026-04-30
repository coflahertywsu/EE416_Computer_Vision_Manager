import QtQuick 2.4
import QtQuick.Controls 2.12

ComboBox {
    id: root

    property int textSize: 30
    property int itemHeight: 60
    property color boxColor: "#404040"
    property color popupColor: "#303030"
    property color borderColor: "#4A90E2"
    property color textColor: "white"
    property color highlightColor: "white"
    property color highlightTextColor: "black"
    property int cornerRadius: 20

    width: 800
    height: 120
    padding: 20

    indicator: Canvas {
        id: canvas
        width: 20
        height: 20
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.verticalCenter: parent.verticalCenter

        Component.onCompleted: requestPaint()

        onPaint: {
            var ctx = getContext("2d")
            ctx.reset()

            ctx.fillStyle = "white"   // ← THIS is your arrow color

            ctx.beginPath()
            ctx.moveTo(0, 6)
            ctx.lineTo(width, 6)
            ctx.lineTo(width / 2, height)
            ctx.closePath()
            ctx.fill()
        }
    }

    background: Rectangle {
        radius: root.cornerRadius
        color: root.boxColor
        border.color: root.borderColor
        border.width: 2
    }

    contentItem: Text {
        text: root.displayText
        color: root.textColor
        font.pixelSize: root.textSize
        verticalAlignment: Text.AlignVCenter
    }

    delegate: ItemDelegate {
        width: root.width
        height: root.itemHeight
        text: modelData
        highlighted: root.highlightedIndex === index

        background: Rectangle {
            color: highlighted ? root.highlightColor : root.popupColor
        }

        contentItem: Text {
            text: parent.text
            color: highlighted ? root.highlightTextColor : root.textColor
            font.pixelSize: root.textSize
            verticalAlignment: Text.AlignVCenter
        }
    }

    popup: Popup {
        y: root.height
        width: root.width
        implicitHeight: contentItem.implicitHeight

        background: Rectangle {
            color: root.popupColor
            radius: 8
            border.color: root.borderColor
            border.width: 2
        }

        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight + 20
            model: root.popup.visible ? root.delegateModel : null
            currentIndex: root.highlightedIndex
        }
    }
}
