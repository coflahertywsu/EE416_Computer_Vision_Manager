// import QtQuick 2.4
// import QtQuick.Controls 1.1

// Item {
//     id: homePage

//     property var pageLoader

//     Rectangle {
//         id: buttonContainer
//         anchors.fill: parent
//         color: "#303030"

//         Button {
//             id: modelUploadBtn
//             text: "Upload New Model"
//             width: 800
//             height: 150
//             anchors.horizontalCenter: parent.horizontalCenter
//             anchors.verticalCenter: parent.verticalCenter
//             anchors.verticalCenterOffset: 200
//             // onClicked: pageLoader.source = "ModelUploadPage.qml"
//         }

//         Button {
//             id: remoteVisionConfigBtn
//             text: "Remote Vision System Configuration"
//             width: 800
//             height: 150
//             anchors.centerIn: parent
//             // onClicked: pageLoader.source = "RemoteVisionConfigPage.qml"
//         }

//         Button {
//             id: liveViewerBtn
//             text: "Live Viewer"
//             width: 800
//             height: 150
//             anchors.horizontalCenter: parent.horizontalCenter
//             anchors.verticalCenter: parent.verticalCenter
//             anchors.verticalCenterOffset: -200
//             onClicked: {
//                 console.log("Live Viewer Clicked")
//                 pageLoader.source = "LiveViewerPage.qml"
//             }
//         }
//     }
// }


import QtQuick 2.4

Item {
    id: homePage

    property var pageLoader

    // Background
    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#3a3a3a" }
            GradientStop { position: 1.0; color: "#1f1f1f" }
        }
    }

    // Title
    Text {
        text: "Remote Vision Control"
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 40
        font.pixelSize: 42
        font.bold: true
        color: "white"
    }

    // Subtitle
    Text {
        text: "Monitor • Configure • Deploy"
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 90
        font.pixelSize: 18
        color: "#bbbbbb"
    }

    // Button container
    Column {
        spacing: 40
        anchors.centerIn: parent

        // ---------- Live Viewer ----------
        Rectangle {
            id: liveCard
            width: 850
            height: 150
            radius: 20
            color: "#404040"
            border.color: "#4a90e2"
            border.width: 2

            Text {
                text: "Live Viewer"
                anchors.centerIn: parent
                font.pixelSize: 30
                color: "white"
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true

                onEntered: liveCard.color = "#505050"
                onExited: liveCard.color = "#404040"

                onPressed: liveCard.color = "#4a4a4a"
                onReleased: {
                    liveCard.color = containsMouse ? "#505050" : "#404040"
                    console.log("Live Viewer Clicked")
                    pageLoader.source = "LiveViewerPage.qml"
                }
            }
        }

        // ---------- Remote Config ----------
        Rectangle {
            id: configCard
            width: 850
            height: 150
            radius: 20
            color: "#404040"
            border.color: "#f5a623"
            border.width: 2

            Text {
                text: "Remote Vision System Configuration"
                anchors.centerIn: parent
                width: parent.width - 40
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 26
                color: "white"
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true

                onEntered: configCard.color = "#505050"
                onExited: configCard.color = "#404040"

                onPressed: configCard.color = "#4a4a4a"
                onReleased: {
                    configCard.color = containsMouse ? "#505050" : "#404040"
                    // pageLoader.source = "RemoteVisionConfigPage.qml"
                }
            }
        }

        // ---------- Upload Model ----------
        Rectangle {
            id: uploadCard
            width: 850
            height: 150
            radius: 20
            color: "#404040"
            border.color: "#7ed321"
            border.width: 2

            Text {
                text: "Upload New Model"
                anchors.centerIn: parent
                font.pixelSize: 30
                color: "white"
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true

                onEntered: uploadCard.color = "#505050"
                onExited: uploadCard.color = "#404040"

                onPressed: uploadCard.color = "#4a4a4a"
                onReleased: {
                    uploadCard.color = containsMouse ? "#505050" : "#404040"
                    // pageLoader.source = "ModelUploadPage.qml"
                }
            }
        }
    }
}
