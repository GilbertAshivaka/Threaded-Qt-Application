import QtQuick
import QtQuick.Window
import QtQuick.Controls.Material
import com.primecounter 1.0

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Prime Counter")

    PrimeCounter{
        id: primeCounter
        onResultTextChanged: {
            resultText : primeCounter.resultText
        }
        onFilterTextChanged: {
            filterText : primeCounter.filterText
        }
        onStarted: {
            startButton.text = "Cancel"
            combobox.enabled = false
            progressBar.value = progress
        }
        onDone: {
            startButton.text = "Start"
            combobox.enabled = true
            progressBar.value = 0
        }
        onProgressChanged: {
            progressBar.value = primeCounter.progress
        }
    }

    Column{
        spacing: 10
        padding: 20
        anchors.fill: parent

        Text{
            text:  "Select a reducing option and measure the speed: "
            font.pixelSize: 16
        }

        ComboBox{
            id: combobox
            width: parent.width* .75
            height: 44
            model:["Unodered reduced", "Ordered reduced"]
            onCurrentIndexChanged:{
                primeCounter.changeCurrentReducedOptions(combobox.currentText)
            }
        }

        Rectangle{
            id: startProgressContainer
            width: parent.width
            height: 50

            Button{
                id: startButton
                text: "Start"
                anchors{
                    left: parent.left
                    verticalCenter: parent.verticalCenter
                }

                onClicked: {
                    primeCounter.start()
                }
            }

            ProgressBar{
                id: progressBar
                width: 300
                value: primeCounter.progress
                to: slider.value
                from: 0
                anchors{
                    left: startButton.right
                    leftMargin: 10
                    verticalCenter: parent.verticalCenter
                }

            }
        }

        Text{
            id: filterText
            text: primeCounter.filterText//"Filter %5 took 2ms to calculate." //select the filter option // selected reduce option
            font.pointSize: 14
        }


        Rectangle{
            id: sliderRect
            height: 40
            width: parent.width

            Slider{
                id: slider
                from: 1
                to: 100000000
                stepSize: 1
                anchors{
                    left: parent.left
                    verticalCenter: parent.verticalCenter
                }

                Timer{
                    id: fillElementsTimer
                    interval: 100
                    repeat: true
                    running: false
                    onTriggered: {
                        primeCounter.setStepSize(Math.round(slider.value))
                    }
                }

                onValueChanged: {
                    fillElementsTimer.start()
                }
            }

            Text{
                id: sizeText
                text: "Elements in list: " + slider.value
                font.pixelSize: 16
                anchors{
                    left: slider.right
                    leftMargin: 10
                    verticalCenter: slider.verticalCenter
                }
            }
        }

        Text{
            id: resultText
            text: primeCounter.resultText//"Found %1 primes in the range of elements"
            font.pointSize: 14
        }
    }
}

















