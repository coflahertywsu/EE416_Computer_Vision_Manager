import QtQuick 2.4
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3 as QtDialogs

Item {
    id: systemSettingsPage

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
        text: "System Settings"
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 20
        font.pixelSize: 42
        font.bold: true
        color: "white"
    }

    // Section Parent Level
    Column {
        spacing: 40
        anchors.centerIn: parent

        //Connection Mode Section
        Column {
            spacing: 20

            Text {
                text: "Connection Mode"
                color: "white"
                font.pixelSize: 30
                font.bold: true
            }

            Row {
                spacing: 80
                anchors.horizontalCenter: parent.horizontalCenter

                ButtonGroup {
                    id: connectionModeGroup
                }

                CustomRadioButton {
                    id: autoRadio
                    text: "Automatic"
                    height: 75
                    textSize: 26
                    checked: true
                    ButtonGroup.group: connectionModeGroup
                }

                CustomRadioButton {
                    id: manualRadio
                    text: "Manual"
                    height: 75
                    textSize: 26
                    ButtonGroup.group: connectionModeGroup
                }

                TextField {
                    id: jetsonIpField
                    opacity: manualRadio.checked
                    enabled: manualRadio.checked
                    width: 650
                    height: 75
                    font.pixelSize: 26
                    color: "white"
                    placeholderText: "Remote Device IP Address (e.g. 192.168.1.1)"

                    background: Rectangle {
                        radius: 10
                        color: "#404040"
                        border.color: "#4A90E2"
                        border.width: 2
                    }
                }
            }
        }

        // Local Repo Section
        Column {
            spacing: 20

            Text {
                text: "Local Model Repository"
                color: "white"
                font.pixelSize: 30
                font.bold: true
            }

            Column {
                spacing: 10

                Text {
                    text: "Models:"
                    color: "white"
                    font.pixelSize: 26
                }

                Row {
                    spacing: 20

                    CustomComboBox {
                        id: localModelComboBox
                        width: 650
                        height: 75
                        textSize: 26
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

            CardButton {
                id: transferToRemoteCard
                text: "Transfer to Remote"
                width: 300
                onClicked: {
                    dirTransfer.setDirToCopy(localModelComboBox.currentIndex)

                    if (!dirTransfer.selectedDirIsValid()) {
                        invalidModelDialog.open()
                        return
                    }

                    if (dirTransfer.remoteCopyExists()) {
                        overwriteRemoteDecisionDialog.open()
                        return
                    }

                    if(dirTransfer.transferToRemote()) {
                        remoteCvModel.loadModelNames()
                        transferSuccessDialog.open()
                    }
                    else {
                        copyFailedDialog.open()
                    }
                }
            }
        }

        // Remote Repo Section
        Column {
            spacing: 20

            Text {
                text: "Remote Model Repository"
                color: "white"
                font.pixelSize: 30
                font.bold: true
            }

            Column {
                spacing: 10

                Text {
                    text: "Models:"
                    color: "white"
                    font.pixelSize: 26
                }

                Row {
                    spacing: 20

                    CustomComboBox {
                        id: remoteModelComboBox
                        width: 650
                        height: 75
                        textSize: 26
                        model: remoteCvModel.cvModelNames
                        onCurrentIndexChanged: {
                            remoteCvModel.setSelectedModel(remoteModelComboBox.currentIndex)
                        }
                    }

                    CardButton {
                        id: buildCard
                        text: remoteCvModel.hasTrtEngine ? "Rebuild..." : "Build..."
                        onClicked: {
                            buildEngineDecisionDialog.open()
                        }
                    }
                }

                Text {
                    text: "Selected model requires TRT engine. Click 'Build' to build engine."
                    visible: !remoteCvModel.hasTrtEngine
                    color: "red"
                    font.pixelSize: 22
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


    ///---------------------------------------------
    /// Dialog Boxes
    ///---------------------------------------------

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


    // Dialogs associated with local repo section
    CustomDialog {
        id: transferSuccessDialog
        parent: systemSettingsPage
        titleText: "Info"
        messageText: "Model transfered successfully. The TensorRT engine will need to be built before the model can be used. This can take several minutes. Build it?"
        confirmText: "Yes"
        cancelText: "No"
        showCancel: true
        accentColor: "white"
        // Note the plumbing here. It's a little indirect, but saves a bunch of code/work.
            // 1. Setting the index of remoteModelComboBox triggers onIndexChaned slot
            // 2. The onIndexChangedSlot sets the selected model of remoteCvModel
        onAccepted: {
            var modelName = localModelComboBox.currentText
            var remoteComboIdx = remoteModelComboBox.model.indexOf(modelName)

            if(remoteComboIdx === -1) {
                buildEngineFailedDialog.open()
                return
            }

            remoteModelComboBox.currentIndex = remoteComboIdx
            buildEngineProgressDialog.open()
            remoteCvModel.buildTrtEngine(false)
        }
    }


    CustomDialog {
        id: copyFailedDialog
        parent: systemSettingsPage
        titleText: "Copy Failed"
        messageText: "Copy Failed"
        confirmText: "OK"
        showCancel: false
        accentColor: "#E74C3C"
    }

    CustomDialog {
        id: invalidModelDialog
        parent: systemSettingsPage
        titleText: "Invalid Model"
        messageText: "Selected folder must contain a model file (.onnx) and labels.txt."
        confirmText: "OK"
        showCancel: false
        accentColor: "#E74C3C"
    }

    CustomDialog {
        id: overwriteLocalDecisionDialog
        parent: systemSettingsPage
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
        parent: systemSettingsPage
        titleText: "Overwrite?"
        messageText: "A model with the same name already exists in the remote repo. Overwrite it?"
        confirmText: "Yes"
        cancelText: "No"
        showCancel: true
        accentColor: "white"

        onAccepted: {
            if(dirTransfer.transferToRemote(true)) {
                transferSuccessDialog.open()
            }
            else {
                copyFailedDialog.open()
            }
        }

        onRejected: {
            console.log("Remote overwrite cancelled")
        }
    }


    //Dialogs associated with remote repo
    CustomDialog {
        id: buildEngineDecisionDialog
        parent: systemSettingsPage
        titleText: remoteCvModel.hasTrtEngine ? "Rebuild Engine?" : "Build Engine?"
        messageText: remoteCvModel.hasTrtEngine
                     ? "This model already has a TensorRT engine. Rebuilding can take several minutes. Rebuild it?"
                     : "Building the TensorRT engine can take several minutes. Build it now?"
        confirmText: remoteCvModel.hasTrtEngine ? "Rebuild" : "Build"
        cancelText: "Cancel"
        showCancel: true
        accentColor: "white"

        onAccepted: {
            buildEngineProgressDialog.open()
            remoteCvModel.buildTrtEngine(remoteCvModel.hasTrtEngine)
        }
    }

    Connections {
        target: remoteCvModel

        function onEngineBuildResult(success) {
            buildEngineProgressDialog.close()

            if(success)
                buildEngineSuccessDialog.open()
            else
                buildEngineFailedDialog.open()
        }
    }

    CustomDialog {
        id: buildEngineProgressDialog
        parent: systemSettingsPage
        titleText: "Building Engine"
        messageText: "TensorRT engine is being built. This may take a several minutes. Do not close software or turn off remote device."
        confirmText: ""
        showConfirm: false
        showCancel: false
        accentColor: "white"
    }

    CustomDialog {
        id: buildEngineSuccessDialog
        parent: systemSettingsPage
        titleText: "Build Complete"
        messageText: "TensorRT engine was built successfully."
        confirmText: "OK"
        showCancel: false
        accentColor: "white"
    }

    CustomDialog {
        id: buildEngineFailedDialog
        parent: systemSettingsPage
        titleText: "Build Failed"
        messageText: "TensorRT engine build failed."
        confirmText: "OK"
        showCancel: false
        accentColor: "red"
    }
}
