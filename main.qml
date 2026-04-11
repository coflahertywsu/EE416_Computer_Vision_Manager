import QtQuick 2.4
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.3
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.1

import org.freedesktop.gstreamer.GLVideoItem 1.0

ApplicationWindow {
    id: window

    property string remoteVisionStartScript: "/home/artemis/Apps/CVApp/build/startup.sh"
    property string remoteVisionKillScript:  "/home/artemis/Apps/CVApp/build/kill.sh"

    visible: true
    width: 640
    height: 555
    x: 30
    y: 30
    color: "#303030"

    Item {
        anchors.fill: parent

        GstGLVideoItem {
            id: video
            objectName: "videoItem"
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            height: parent.height - 90
        }

        Rectangle {
            id: buttonContainer
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            width: 300
            height: 90
            color: "#303030"


            Button {
                id: startSprayer
                text: "Start"
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                width: 100
                height: 50
                onClicked: {
                    console.log("Start Clicked");
                    remotePipeline.start();
                }
            }

            Button {
                id: stopSprayer
                text: "Stop"
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                width: 100
                height: 50
                onClicked: {
                    remotePipeline.stop();
                }
            }
        }



        // Rectangle {
        //     color: Qt.rgba(1, 1, 1, 0.7)
        //     border.width: 1
        //     border.color: "white"
        //     anchors.bottom: video.bottom
        //     anchors.bottomMargin: 15
        //     anchors.horizontalCenter: parent.horizontalCenter
        //     width : parent.width - 30
        //     height: parent.height - 30
        //     radius: 8

        //     MouseArea {
        //         id: mousearea
        //         anchors.fill: parent
        //         hoverEnabled: true
        //         onEntered: {
        //             parent.opacity = 1.0
        //             hidetimer.start()
        //         }
        //     }

        //     Timer {
        //         id: hidetimer
        //         interval: 1000
        //         onTriggered: {
        //             parent.opacity = 0.0
        //             stop()
        //         }
        //     }
        // }
    }
}

