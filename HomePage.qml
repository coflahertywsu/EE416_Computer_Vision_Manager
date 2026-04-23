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
        anchors.topMargin: 20
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

    // Buttons
    Column {
        spacing: 40
        anchors.centerIn: parent

        CardButton {
            id: liveViewerCard
            text: "Live Viewer"
            width: 850
            height: 150
            border.color: "#4A90E2"
            onClicked: {
                pageLoader.source = "LiveViewerPage.qml"
            }
        }

        CardButton {
            id: remoteConfigCard
            text: "Remote Vision System Configuration"
            width: 850
            height: 150
            border.color: "#F5A623"
            onClicked: {
                //pageLoader.source = "RemoteVisionConfigPage.qml"
            }
        }

        CardButton {
            id: uploadModelCard
            text: "Upload New Model"
            width: 850
            height: 150
            border.color: "#7ED321"
            onClicked: {
                dirTransfer.setLocalRepoWatcher(true)
                pageLoader.source = "UploadModelPage.qml"
            }
        }
    }
}
