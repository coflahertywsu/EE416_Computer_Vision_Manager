import QtQuick 2.4
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.3
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.1

import org.freedesktop.gstreamer.GLVideoItem 1.0

Item {
    id: liveViewerPage

    property var pageLoader

    Rectangle {
        anchors.fill: parent
        color: "#303030"

        GstGLVideoItem {
            id: video
            objectName: "videoItem"
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: buttonContainer.top
            Component.onCompleted: {
                videoController.setVideoItem(video)
                videoController.syncVideo()
            }
        }

        Rectangle {
            id: buttonContainer
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            width: 300
            height: 90
            color: "#303030"

            Button {
                id: startVideoBtn
                text: "Start"
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                width: 100
                height: 50
                onClicked: {
                    console.log("Start Clicked")
                    remotePipeline.start()
                }
            }

            Button {
                id: stopVideoBtn
                text: "Stop"
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                width: 100
                height: 50
                onClicked: {
                    remotePipeline.stop()
                }
            }
        }
    }
}




// import QtQuick 2.4
// import QtQuick.Controls 1.1
// import QtQuick.Controls.Styles 1.3
// import QtQuick.Dialogs 1.2
// import QtQuick.Window 2.1

// import org.freedesktop.gstreamer.GLVideoItem 1.0

// Item {
//     id: liveViewerPage

//     visible: true
//     width: pageLoader.width / 2
//     height: width * (3/4)
//     x: 30
//     y: 30

//     Item {
//         anchors.fill: parent

//         GstGLVideoItem {
//             id: video
//             objectName: "videoItem"
//             anchors.top: parent.top
//             anchors.horizontalCenter: parent.horizontalCenter
//             width: parent.width
//             height: parent.height - 90
//         }

//         Rectangle {
//             id: buttonContainer
//             anchors.horizontalCenter: parent.horizontalCenter
//             anchors.bottom: parent.bottom
//             width: 300
//             height: 90
//             color: "#303030"


//             Button {
//                 id: startVideoBtn
//                 text: "Start"
//                 anchors.left: parent.left
//                 anchors.verticalCenter: parent.verticalCenter
//                 width: 100
//                 height: 50
//                 onClicked: {
//                     console.log("Start Clicked");
//                     remotePipeline.start();
//                 }
//             }

//             Button {
//                 id: stopVideoBtn
//                 text: "Stop"
//                 anchors.right: parent.right
//                 anchors.verticalCenter: parent.verticalCenter
//                 width: 100
//                 height: 50
//                 onClicked: {
//                     remotePipeline.stop();
//                 }
//             }
//         }
//     }
// }


