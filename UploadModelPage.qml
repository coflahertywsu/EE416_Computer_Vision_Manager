import QtQuick 2.4
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3

Item {
    id: uploadModelPage

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
        text: "Upload New Model"
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 20
        font.pixelSize: 42
        font.bold: true
        color: "white"
    }

    Column {
        spacing: 40
        anchors.centerIn: parent

        Row {
            spacing: 20

            ComboBox {
                id: modelComboBox
                width: 800
                height: 120
                padding: 20
                model: dirTransfer.localRepoEntryList

                background: Rectangle {
                    radius: 20
                    color: "#404040"
                    border.color: "#4A90E2"
                    border.width: 2
                }

                contentItem: Text {
                    text: modelComboBox.displayText
                    color: "white"
                    font.pixelSize: 30
                    verticalAlignment: Text.AlignVCenter
                    // leftPadding: 20
                }

                delegate: ItemDelegate {
                    width: modelComboBox.width
                    height: 60
                    text: modelData
                    highlighted: modelComboBox.highlightedIndex === index

                    background: Rectangle {
                        color: highlighted ? "white" : "#303030"
                    }

                    contentItem: Text {
                        text: parent.text
                        color: highlighted ? "black" : "white"
                        font.pixelSize: 30
                        verticalAlignment: Text.AlignVCenter
                        anchors.fill: parent
                        // leftPadding: 20
                    }
                }

                popup: Popup {
                    y: modelComboBox.height
                    width: modelComboBox.width
                    implicitHeight: contentItem.implicitHeight

                    background: Rectangle {
                        color: "#303030"
                        radius: 8
                        border.color: "#4A90E2"
                        border.width: 2
                    }

                    contentItem: ListView {
                        clip: true
                        implicitHeight: contentHeight
                        model: modelComboBox.popup.visible ? modelComboBox.delegateModel : null
                        currentIndex: modelComboBox.highlightedIndex
                    }
                }
            }

            CardButton {
                id: browseCard
                text: "Browse..."
                // width: 200
                // height: 120
                onClicked: {
                    folderDialog.open()
                }
            }
        }

        Row {
            spacing: 40
            anchors.horizontalCenter: parent.horizontalCenter

            CardButton {
                text: "Upload"
                borderColor: "#7ED321"
                onClicked: {
                    console.log("Upload clicked")
                }
            }

            CardButton {
                text: "Back"
                borderColor: "#7ED321"
                onClicked: {
                    dirTransfer.setLocalRepoWatcher(false)
                    pageLoader.source = "HomePage.qml"
                }
            }
        }
    }

    FileDialog {
        id: folderDialog
        title: "Select Model Folder"
        folder: shortcuts.home
        selectFolder: true

        onAccepted: {
            console.log("Selected folder:", folderDialog.folder)
        }
    }
}
