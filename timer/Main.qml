import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Shapes

Window {
    id: window
    width: 420
    height: 520
    visible: true
    color: "#111418"
    title: "Round Timer"

    Item {
        id: root
        anchors.centerIn: parent
        width: 280
        height: 280

        property int totalSeconds: 90
        property int remainingSeconds: 90
        property bool running: false
        property bool editing: false
        property bool pressed: false

        property real timerScale: 1.0
        property color ringBaseColor: "#8A6BFF"
        property color ringOverlayColor: "transparent"


        property real warningProgress: {
            if (remainingSeconds > 5) return 0.0
            if (remainingSeconds <= 0) return 1.0
            return (5 - remainingSeconds) / 5.0
        }


        function formatTime(sec) {
            let m = Math.floor(Math.max(0, sec) / 60)
            let s = Math.max(0, sec) % 60
            return (m < 10 ? "0" + m : m) + ":" + (s < 10 ? "0" + s : s)
        }

        function toggleTimer() {
            if (!editing) running = !running
        }

        function enterEditMode() {
            if (!running) {
                editing = true
                minutesInput.text = Math.floor(remainingSeconds / 60)
                secondsInput.text = remainingSeconds % 60
                minutesInput.forceActiveFocus()
            }
        }

        function leaveEditMode(save) {
            if (save) {
                let m = parseInt(minutesInput.text) || 0
                let s = parseInt(secondsInput.text) || 0
                totalSeconds = m * 60 + Math.min(s, 59)
                remainingSeconds = totalSeconds
            }
            editing = false
        }


        Timer {
            id: countdownTimer
            interval: 1000; repeat: true; running: root.running
            onTriggered: {
                if (root.remainingSeconds > 0) root.remainingSeconds--
                else root.running = false
            }
        }


        Canvas {
            id: ringCanvas
            anchors.fill: parent
            rotation: -90
            
            onPaint: {
                var ctx = getContext("2d")
                ctx.reset()
                var cx = width/2, cy = height/2, r = 110

                ctx.beginPath()
                ctx.arc(cx, cy, r, 0, Math.PI * 2)
                ctx.lineWidth = 8
                ctx.strokeStyle = root.ringBaseColor
                ctx.stroke()


                if (totalSeconds > 0) {
                    ctx.beginPath()
                    ctx.arc(cx, cy, r, 0, (Math.PI * 2) * (remainingSeconds / totalSeconds))
                    ctx.strokeStyle = "white"
                    ctx.lineWidth = 8
                    ctx.stroke()
                }
            }
            
            Connections {
                target: root
                function onRemainingSecondsChanged() { ringCanvas.requestPaint() }
                function onRingBaseColorChanged() { ringCanvas.requestPaint() }
            }
        }

 
        Rectangle {
            anchors.centerIn: parent
            width: 200; height: 200; radius: 100
            color: "#1A212B"
            
            Column {
                anchors.centerIn: parent
                visible: !root.editing
                Text {
                    text: root.formatTime(root.remainingSeconds)
                    color: "white"; font.pixelSize: 48; font.bold: true
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                Text {
                    text: root.running ? "Running" : "Tap to Start"
                    color: root.running ? "#5BE37D" : "#67D4FF"
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }


            Row {
                anchors.centerIn: parent
                visible: root.editing
                spacing: 5
                TextInput {
                    id: minutesInput; color: "white"; font.pixelSize: 36
                    validator: IntValidator { bottom: 0; top: 99 }
                }
                Text { text: ":"; color: "white"; font.pixelSize: 36 }
                TextInput {
                    id: secondsInput; color: "white"; font.pixelSize: 36
                    validator: IntValidator { bottom: 0; top: 59 }
                }
            }
        }


        transform: Scale {
            origin.x: root.width/2; origin.y: root.height/2
            xScale: root.timerScale; yScale: root.timerScale
        }


        state: editing ? "editing" : (pressed ? "pressed" : (running ? "running" : "idle"))

        states: [
            State {
                name: "idle"
                PropertyChanges { target: root; ringBaseColor: "#8A6BFF"; timerScale: 1.0 }
            },
            State {
                name: "pressed"
                PropertyChanges { target: root; timerScale: 0.9 }
            },
            State {
                name: "running"
                PropertyChanges {
                    target: root
                    ringBaseColor: Qt.rgba(0.5 + 0.5 * root.warningProgress, 0.8 - 0.6 * root.warningProgress, 0.5 - 0.5 * root.warningProgress, 1)
                }
            },
            State {
                name: "editing"
                PropertyChanges { target: root; timerScale: 1.1; ringBaseColor: "#67D4FF" }
            }
        ]

        transitions: [
            Transition {
                from: "pressed"; to: "idle"
                SequentialAnimation {
                    NumberAnimation { target: root; property: "timerScale"; to: 1.15; duration: 150; easing.type: Easing.OutBack }
                    NumberAnimation { target: root; property: "timerScale"; to: 1.0; duration: 200 }
                }
            },
            Transition {
                from: "*"; to: "pressed"
                NumberAnimation { target: root; property: "timerScale"; duration: 100 }
            },
            Transition {
                from: "*"; to: "editing"
                NumberAnimation { target: root; property: "timerScale"; duration: 200; easing.type: Easing.OutBack }
            }
        ]


        MouseArea {
            anchors.fill: parent
            onPressed: root.pressed = true
            onReleased: root.pressed = false
            onClicked: root.toggleTimer()
            onDoubleClicked: root.enterEditMode()
        }
        

        tapOutsideCatcher: MouseArea {
            anchors.fill: window.contentItem
            enabled: root.editing
            onClicked: root.leaveEditMode(true)
            z: -1
        }
    }
}
