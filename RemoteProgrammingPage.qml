import QtQuick 2.12
import QtQuick.Controls 2.12
import App 1.0

Item {
    id: remoteProgrammingPage

    property var pageLoader
    property int instructionCount: userProgram.instructionCount()

    Connections {
        target: userProgram
        function onInstructionsChanged() {
            instructionCount = userProgram.instructionCount()
            instructionListView.model = instructionCount
        }
    }

    Component.onCompleted: {
        if (!userProgram.load())
            console.log("Remote Programming Page: Failed to load user program from json.")

        if (userProgram.instructionCount() === 0)
            userProgram.addInstruction()

        instructionCount = userProgram.instructionCount()
    }

    // Background
    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#3a3a3a" }
            GradientStop { position: 1.0; color: "#1f1f1f" }
        }
    }

    // Top level container
    Column {
        spacing: 60
        anchors.centerIn: parent

        // Title
        Text {
            text: "Remote Vision System Programming"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 20
            font.pixelSize: 42
            font.bold: true
            color: "white"
        }

        // Splits screen into two sub columns
        Row {
            spacing: 60

            // Left side
            Column {
                spacing: 20

                // Available Models for Profile section
                Column {
                    spacing: 15

                    Text {
                        text: "Available Models"
                        color: "white"
                        font.pixelSize: 30
                        font.bold: true
                    }

                    Row {
                        spacing: 40

                        CustomComboBox {
                            id: availableModelsComboBox
                            width: 500
                            height: 65
                            textSize: 26
                            model: userProgram.cvModelNames
                            onCurrentIndexChanged: {
                                userProgram.selectCvModelConfig(currentIndex)
                            }
                        }
                    }
                }

                // Confidence Threshold
                Column {
                    spacing: 15

                    Text {
                        text: "Confidence Threshold"
                        font.pixelSize: 26
                        color: "white"
                    }

                    Row {
                        spacing: 40

                        Slider {
                            id: preThresholdSlider
                            width: 420
                            from: 0.0
                            to: 1.0
                            stepSize: 0.01
                            value: userProgram.selectedCvModelConfig.preThreshold
                            background: Rectangle {
                                color: "#4A90E2"
                            }

                            onValueChanged: {
                                userProgram.selectedCvModelConfig.preThreshold = value
                            }
                        }

                        Text {
                            text: preThresholdSlider.value.toFixed(2)
                            font.pixelSize: 22
                            color: "white"
                        }
                    }
                }

                // Overlap Sensitivity
                Column {
                    spacing: 15

                    Text {
                        text: "Overlap Sensitivity"
                        font.pixelSize: 26
                        color: "white"
                    }

                    Row {
                        spacing: 40

                        Slider {
                            id: nmsThresholdSlider
                            width: 420
                            from: 0.0
                            to: 1.0
                            stepSize: 0.01
                            value: userProgram.selectedCvModelConfig.nmsIouThreshold
                            background: Rectangle {
                                color: "#4A90E2"
                            }

                            onValueChanged: {
                                userProgram.selectedCvModelConfig.nmsIouThreshold = value
                            }
                        }

                        Text {
                            text: nmsThresholdSlider.value.toFixed(2)
                            font.pixelSize: 22
                            color: "white"
                        }
                    }
                }
            }

            // Right side
            Column {
                spacing: 20
                width: 850

                Text {
                    text: "Instructions"
                    color: "white"
                    font.pixelSize: 30
                    font.bold: true
                }

                // Profile Instructions
                Row {
                    spacing: 15

                    ListView {
                        id: instructionListView
                        width: 850
                        height: currentItem ? currentItem.height : 460
                        spacing: 25
                        clip: true
                        model: userProgram.instructionCount()

                        snapMode: ListView.SnapOneItem
                        highlightRangeMode: ListView.StrictlyEnforceRange
                        preferredHighlightBegin: 0
                        preferredHighlightEnd: height
                        highlightMoveDuration: 200

                        delegate: Rectangle {
                            property int instructionIndex: index
                            property int inputCount: userProgram.instructionAt(index).inputCount


                            width: 820
                            height: 400 + inputCount * 60
                            radius: 20
                            color: "#303030"
                            border.color: "#4A90E2"
                            border.width: 2

                            Column {
                                anchors.fill: parent
                                anchors.margins: 25
                                spacing: 15

                                Text {
                                    text: (index + 1) + " of " + userProgram.instructionCount()
                                    color: "white"
                                    font.pixelSize: 28
                                    font.bold: true
                                    width: 520
                                }

                                Text {
                                    text: "WHEN"
                                    color: "#4A90E2"
                                    font.pixelSize: 24
                                    font.bold: true
                                }

                                Repeater {
                                    model: {
                                        var instr = userProgram.instructionAt(instructionIndex)
                                        return instr ? instr.inputCount : 0
                                    }

                                    delegate: Row {
                                        spacing: 15

                                        property var instruction: userProgram.instructionAt(instructionIndex)
                                        property var input: instruction ? instruction.inputAt(index) : null

                                        CustomComboBox {
                                            id: inputTypeComboBox
                                            width: 200
                                            height: 65
                                            textSize: 21
                                            model: ["CV Object", "Digital Input", "Latch"]

                                            currentIndex: {
                                                if(!input) return 0

                                                return input.inputType
                                            }
                                            onCurrentIndexChanged: {
                                                if (!input) return

                                                if (input.inputType !== currentIndex)
                                                    input.inputType = currentIndex
                                            }
                                        }

                                        CustomComboBox {
                                            id: inputSelctionComboBox
                                            width: 200
                                            height: 65
                                            textSize: 21
                                            model: {
                                                switch (inputTypeComboBox.currentIndex) {
                                                case 0:
                                                    return userProgram.selectedCvModelConfig.labels
                                                case 1:
                                                    return ["Input 1", "Input 2", "Input 3", "Input 4"]
                                                case 2:
                                                default:
                                                    return ["Latch A", "Latch B", "Latch C", "Latch D"]
                                                }
                                            }

                                            currentIndex: {
                                                if (!input) return 0

                                                if (inputTypeComboBox.currentIndex === 0) {
                                                    var idx = model.indexOf(input.labelText)
                                                    return idx >= 0 ? idx : 0
                                                }

                                                return input.inputChannel
                                            }

                                            onCurrentIndexChanged: {
                                                if(!input) return

                                                if (inputTypeComboBox.currentIndex === 0)
                                                    input.labelText = model[currentIndex]
                                                else
                                                    input.inputChannel = currentIndex
                                            }
                                        }

                                        CustomComboBox {
                                            id: inputPolarityComboBox
                                            width: 200
                                            height: 65
                                            textSize: 21
                                            model: ["is active", "is NOT active"]

                                            currentIndex: {
                                                if(!input) return 0

                                                return input.activeHigh ? 0 : 1
                                            }

                                            onCurrentIndexChanged: {
                                                if(!input) return

                                                input.activeHigh = (currentIndex === 0)
                                            }
                                        }
                                    }
                                }

                                Row {
                                    spacing: 15

                                    CardButton {
                                        id: addInputCard
                                        text: "+ Add Condition"
                                        width: 200
                                        height: 60
                                        fontSize: 21
                                        onClicked: {
                                            var instruction = userProgram.instructionAt(index)
                                            if (!instruction) return

                                            instruction.addInput()
                                        }
                                    }

                                    CardButton {
                                        id: deleteInputCard
                                        text: "– Delete Last"
                                        width: 200
                                        height: 60
                                        fontSize: 21
                                        enabled: userProgram.instructionAt(index).inputCount > 1
                                        onClicked: {
                                            var instruction = userProgram.instructionAt(index)
                                            if (!instruction || instruction.inputCount < 2) return

                                            instruction.removeInput(instruction.inputCount - 1)
                                        }
                                    }

                                }

                                Text {
                                    text: "DO"
                                    color: "#4A90E2"
                                    font.pixelSize: 24
                                    font.bold: true
                                }

                                Row {
                                    spacing: 15

                                    CustomComboBox {
                                        id: outputTypeComboBox
                                        width: 200
                                        height: 65
                                        textSize: 21
                                        model: ["Digital Output", "Latch"]

                                        currentIndex: {
                                            var instruction = userProgram.instructionAt(index)
                                            if (!instruction || !instruction.output) return 0

                                            return instruction.output.outputType
                                        }

                                        onCurrentIndexChanged: {
                                            var instruction = userProgram.instructionAt(index)
                                            if (!instruction) return

                                            if (!instruction || !instruction.output) return

                                            instruction.output.outputType = currentIndex
                                        }
                                    }

                                    CustomComboBox {
                                        id: outputSelectionComboBox
                                        width: 200
                                        height: 65
                                        textSize: 21
                                        model: {
                                            if(outputTypeComboBox.currentIndex === 0)
                                                return ["Output 1", "Output 2", "Output 3", "Output 4"]
                                            else
                                                return ["Latch A", "Latch B", "Latch C", "Latch D"]
                                        }

                                        currentIndex: {
                                            var instruction = userProgram.instructionAt(index)
                                            if (!instruction || !instruction.output) return 0

                                            return instruction.output.outputChannel
                                        }

                                        onCurrentIndexChanged: {
                                            var instruction = userProgram.instructionAt(index)
                                            if (!instruction) return

                                            if (!instruction || !instruction.output) return

                                            instruction.output.outputChannel = currentIndex
                                        }
                                    }

                                    CustomComboBox {
                                        id: outputFunctionComboBox
                                        width: 200
                                        height: 65
                                        textSize: 21
                                        model: {
                                            if(outputTypeComboBox.currentIndex === 0)
                                                return ["Output ON", "Pulse Output"]
                                            else
                                                return ["Set Latch", "Reset Latch"]
                                        }

                                        currentIndex: {
                                            var instruction = userProgram.instructionAt(index)
                                            if (!instruction || !instruction.output) return 0

                                            return instruction.output.outputFunction
                                        }

                                        onCurrentIndexChanged: {
                                            var instruction = userProgram.instructionAt(index)
                                            if (!instruction) return

                                            if (!instruction || !instruction.output) return

                                            instruction.output.outputFunction = currentIndex
                                        }
                                    }

                                    TextField {
                                        id: pulseTimeTextField
                                        width: 130
                                        height: 65
                                        visible: (outputFunctionComboBox.currentIndex === 1) && (outputTypeComboBox.currentIndex === 0)
                                        enabled: visible
                                        font.pixelSize: 21
                                        color: "white"
                                        placeholderText: "ms"

                                        validator: IntValidator { bottom: 0 }

                                        text: {
                                            var instruction = userProgram.instructionAt(index)
                                            if (!instruction || !instruction.output) return ""

                                            return instruction.output.pulseTimeMs > 0 ? instruction.output.pulseTimeMs : ""
                                        }

                                        background: Rectangle {
                                            radius: 10
                                            color: "#404040"
                                            border.color: "#4A90E2"
                                            border.width: 2
                                        }

                                        onTextChanged: {
                                            var instruction = userProgram.instructionAt(index)
                                            if (!instruction || !instruction.output) return

                                            if (outputTypeComboBox.currentIndex === 0 &&
                                                    outputFunctionComboBox.currentIndex === 1)
                                            {
                                                var val = parseInt(text)
                                                if (!isNaN(val))
                                                    instruction.output.pulseTimeMs = val
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    Column {
                        spacing: 20
                        anchors.verticalCenter: parent.verticalCenter

                        CardButton {
                            text: "▲"
                            width: 80
                            height: 80
                            fontSize: 30
                            enabled: instructionListView.currentIndex > 0
                            onClicked: instructionListView.currentIndex--
                        }

                        CardButton {
                            text: "▼"
                            width: 80
                            height: 80
                            fontSize: 30
                            enabled: instructionListView.currentIndex < instructionCount - 1
                            onClicked: instructionListView.currentIndex++
                        }
                    }
                }

                Row {
                    spacing: 25

                    CardButton {
                        id: addInstructionCard
                        text: "+ Add Instruction"
                        width: 260
                        height: 65
                        fontSize: 24
                        onClicked: userProgram.addInstruction()
                    }

                    CardButton {
                        id: deleteLastInstructionCard
                        text: "– Delete Last"
                        width: 260
                        height: 65
                        fontSize: 24
                        enabled: instructionCount > 1
                        onClicked: {
                            if (instructionCount > 0)
                                userProgram.removeInstruction(instructionCount - 1)
                        }
                    }
                }
            }
        }

        CardButton {
            text: "Back"
            width: 300
            height: 65
            anchors.horizontalCenter: parent.horizontalCenter
            borderColor: "#7ED321"
            onClicked: {
                if(!userProgram.save())
                    console.log("Remote Program Page: Failed to save user program to json.");

                pageLoader.source = "HomePage.qml"
            }
        }
    }
}

