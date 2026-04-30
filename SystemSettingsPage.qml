import QtQuick 2.4
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3 as QtDialogs

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

    // Controls (Buttons and Combobox)
    Column {
        spacing: 40
        anchors.centerIn: parent

        Column {
            spacing: 10

            Text {
                text: "Select Model:"
                color: "white"
                font.pixelSize: 30   // slightly smaller than combo text
            }

            Row {
                spacing: 20

                CustomComboBox {
                    id: modelComboBox
                    width: 800
                    height: 120
                    textSize: 30
                    model: dirTransfer.localRepoEntryList
                }

                CardButton {
                    id: browseCard
                    text: "Browse..."
                    onClicked: {
                        folderDialog.open()
                    }
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
                    dirTransfer.setDirToCopy(modelComboBox.currentIndex)

                    if (!dirTransfer.selectedDirIsValid()) {
                        invalidModelDialog.open()
                        return
                    }

                    if (dirTransfer.remoteCopyExists()) {
                        overwriteRemoteDecisionDialog.open()
                        return
                    }

                    if(dirTransfer.transferToRemote()) {
                        uploadSuccessDialog.open()
                    }
                    else {
                        copyFailedDialog.open()
                    }
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

    // Popup Dialog Windows
    QtDialogs.FileDialog {
        id: folderDialog
        title: "Select Model Folder"
        folder: shortcuts.home
        selectFolder: true
        selectMultiple: false
        onAccepted: {
            var path = folder.toString().replace("file:///", "")
            dirTransfer.setDirToCopy(path)

            if (!dirTransfer.selectedDirIsValid()) {
                invalidModelDialog.open()
                return
            }

            if (dirTransfer.localRepoCopyExists()) {
                overwriteLocalDecisionDialog.open()
                return
            }

            if(!dirTransfer.copyToLocalRepo(false)) {
                copyFailedDialog.open()
            }
        }
    }

    CustomDialog {
        id: uploadSuccessDialog
        parent: uploadModelPage
        titleText: "Info"
        messageText: "Model uploaded successfully."
        confirmText: "OK"
        showCancel: false
        accentColor: "white"
    }

    CustomDialog {
        id: copyFailedDialog
        parent: uploadModelPage
        titleText: "Copy Failed"
        messageText: "Copy Failed"
        confirmText: "OK"
        showCancel: false
        accentColor: "#E74C3C"
    }

    CustomDialog {
        id: invalidModelDialog
        parent: uploadModelPage
        titleText: "Invalid Model"
        messageText: "Selected folder must contain a model file (.onnx/.pt) and labels.txt."
        confirmText: "OK"
        showCancel: false
        accentColor: "#E74C3C"
    }

    CustomDialog {
        id: overwriteLocalDecisionDialog
        parent: uploadModelPage
        titleText: "Overwrite?"
        messageText: "A model with the same name already exists in the local repo. Overwrite it?"
        confirmText: "Yes"
        cancelText: "No"
        showCancel: true
        accentColor: "white"

        onAccepted: {
            if(!dirTransfer.copyToLocalRepo(true)) {
                copyFailedDialog.open()
            }
        }

        onRejected: {
            console.log("Local overwrite cancelled")
        }
    }

    CustomDialog {
        id: overwriteRemoteDecisionDialog
        parent: uploadModelPage
        titleText: "Overwrite?"
        messageText: "A model with the same name already exists in the remote repo. Overwrite it?"
        confirmText: "Yes"
        cancelText: "No"
        showCancel: true
        accentColor: "white"

        onAccepted: {
            if(dirTransfer.transferToRemote(true)) {
                uploadSuccessDialog.open()
            }
            else {
                copyFailedDialog.open()
            }
        }

        onRejected: {
            console.log("Remote overwrite cancelled")
        }
    }
}
