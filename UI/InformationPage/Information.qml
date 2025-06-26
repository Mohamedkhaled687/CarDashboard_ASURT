import QtQuick 2.15
import QtQuick.Shapes
import "../StatusBar"

Rectangle {
    id: root

    property real scaleFactor : 1.0
    property string sessionName: ""
    property string portNumber: ""
    property string portName: ""
    property int baudRate: 0
    property bool isSerialSource: false
    property real maxLateralG: 3.5  // Maximum lateral G-force (cornering)
    property real maxLongitudinalG: 2.0  // Maximum longitudinal G-force (acceleration)
    property real maxBrakingG: 3.5  // Maximum braking G-force
    property real xDiagram: (communicationManager.lateralG / maxLateralG) * (ggImage.width / 2 - 20)
    property real yDiagram: (communicationManager.longitudinalG / maxLongitudinalG) * (ggImage.height / 2 - 20)

    color: "#1A3438"
    radius: 40
    border.color: "#A6F1E0"
    border.width: 5

        /******* Status Bar *********/

    StatusBar {
        id: statusBar
        nameofsession : root.sessionName
        nameOfport : communicationManager.isSerialSource ? root.portName + " (" + root.baudRate + ")" : root.portNumber
   }




    /************  Steering Wheel and Proximity Sener ************/

    Rectangle {
        id : leftRect
        width : parent.width / 4
        height : parent.height - 20 * root.scaleFactor
        color : "#09122C"
        radius : 30
        border.width : 2
        border.color : "#D84040"

        anchors {
            top : statusBar.bottom
            left: parent.left
            bottom : parent.bottom
            leftMargin : 12
            topMargin : 5
            bottomMargin : 12
        }

        Rectangle {
            id: steeringWheelRect
            width: parent.width - 40
            height: parent.height / 3
            color: "#636363"
            border.color: "turquoise"
            border.width: 2
            radius: 20
            anchors {
                horizontalCenter : parent.horizontalCenter
                top : parent.top
                topMargin : 10

            }
            SteeringWheel {
                id: steeringWheel
                encoderAngle : communicationManager ? communicationManager.encoderAngle : 0
                anchors.centerIn: parent
            }
        }


        Rectangle {
            id : proximityRect
            width : parent.width - 30
            height : parent.height / 2
            color : "#636363"
            border.color: "turquoise"
            border.width: 2
            radius: 20

            anchors {
                horizontalCenter : parent.horizontalCenter
                top : steeringWheelRect.bottom
                bottom : parent.bottom
                topMargin : 10
                bottomMargin : 10
            }

            Image {
                id : car
                source : "../Assets/AI_car_transparent.png"
                anchors.centerIn : parent
                height : parent.height - 30
                fillMode : Image.PreserveAspectFit
                smooth : true
            }
            WheelSpeed {
                id: fl
                wheelPos: "FL"
                scaleFactor : 1.1
                currentSpeed: communicationManager ? communicationManager.speedFL : 0
                anchors {
                    top : car.top
                    left : proximityRect.left
                    topMargin : 70
                    leftMargin : 20
                }
            }
            WheelSpeed {
                id: fr
                wheelPos: "FR"
                scaleFactor : 1.1
                currentSpeed: communicationManager ? communicationManager.speedFR : 0
                anchors {
                    top : car.top
                    right : proximityRect.right
                    topMargin : 70
                    rightMargin : 20
                }
            }

            WheelSpeed {
                id: bl
                wheelPos: "BL"
                scaleFactor : 1.1
                currentSpeed: communicationManager ? communicationManager.speedBL : 0
                anchors {
                    bottom : car.bottom
                    left : proximityRect.left
                    bottomMargin : 70
                    leftMargin : 20
                }
            }

            WheelSpeed {
                id: br
                wheelPos: "BR"
                scaleFactor : 1.1
                currentSpeed: communicationManager ? communicationManager.speedBR : 0
                anchors {
                    bottom : car.bottom
                    right : proximityRect.right
                    bottomMargin : 70
                    rightMargin : 20
                }
            }
        }
    }

    /********************************************************/




    /******** Meters Screen *******/

    Rectangle {
        id: metersScreen
        width: speedometer.width + rpmMeter.width - 45
        height: 335
        color: "#09122C"
        border.color: "#D84040"
        border.width: 2

        anchors {
            top : statusBar.bottom
            topMargin: 8
            left : leftRect.right
            leftMargin : 70

        }

        radius: 20

        Speedometer {
            id: speedometer
            speed: communicationManager ? communicationManager.speed : 0
            anchors {
                left: parent.left
                leftMargin: -20
                verticalCenter: parent.verticalCenter
                top: parent.top
            }
        }

        RpmMeter {
            id: rpmMeter
            rpm: communicationManager ? communicationManager.rpm : 0
            anchors {
                left: speedometer.right
                right: parent.right
                rightMargin: 30
                verticalCenter: parent.verticalCenter
            }
        }
    }


    /******** Battery , Accelator , Braker Pedal Readings *************/

    Rectangle {
        id : pedalTempRect
        width : parent.width / 5.1
        height : parent.height / 3.5

        color: "#09122C"
        border.color: "#D84040"
        border.width: 2
        radius : 20

        anchors {
            top : metersScreen.bottom
            topMargin : 65
            left : leftRect.right
            leftMargin : 30
        }

        AcceleratorPedal {
            id: acceleratorPedal
            pedalPosition:  communicationManager ? communicationManager.accPedal : 0
            anchors {
                bottom: parent.bottom
                left: parent.left
                margins: 10
            }
        }

        BrakePadel {
            id: brakePedal
            pedalPosition: communicationManager ? communicationManager.brakePedal : 0
            anchors {
                bottom: parent.bottom
                left: acceleratorPedal.right
                margins: 10
            }
        }

        TemperatureIndicator {
            id: temperatureIndicator
            temperature: communicationManager ? communicationManager.temperature : 0
            anchors {
                top: parent.top
                left: parent.left
                leftMargin: 10
                topMargin: 20
            }
        }

        BatteryLevelIndicator {
            id: batteryLevelIndicator
            scaleFactor: parent.height / 222.22
            batteryLevel: communicationManager ? communicationManager.batteryLevel : 0
            anchors {
                right: parent.right
                top: parent.top
                rightMargin: 80
                topMargin: 75
            }
        }
    }
    /********************************************************/


    /****** IMU *****/

    // Replacement for the bottomRect section only in the main QML file

    Rectangle {
        id: bottomRect

        width: parent.width / 3
        height: parent.height / 2.8
        color: "#09122C"
        border.color: "#D84040"
        border.width: 2
        radius: 30

        anchors {
            top: metersScreen.bottom
            left: pedalTempRect.right
            right: rightRect.left
            bottom: parent.bottom
            margins: 10
        }



        Image {
            id: ggImage
            source: "../Assets/GG_Diagram.png"
            width: 294
            height: 294
            anchors.centerIn: parent
            anchors.margins: 8
            rotation: -90
            fillMode: Image.PreserveAspectFit
            smooth: true

            // Calculate center point dynamically
            property real centerX: width / 2
            property real centerY: height / 2



            Shape {
                id: pathShape
                anchors.fill: parent
                smooth: true
                antialiasing: true

                ShapePath {
                    id: movementPath
                    strokeWidth: 2
                    strokeColor: "white"
                    strokeStyle: ShapePath.SolidLine
                    fillColor: "transparent"

                    // Start at the center point
                    startX: ggImage.centerX
                    startY: ggImage.centerY

                    // Create path segments using a dynamic PathPolyline
                    PathPolyline {
                        id: polyline
                    }
                }
            }

            // Add the point marker
            Image {
                id: pointImage
                source: "../Assets/point.png"
                width: 20
                height: 20

                // Position at the center of the marker with limits
                x: Math.max(0, Math.min(ggImage.width - width, root.yDiagram + ggImage.centerX - width/2))
                y: Math.max(0, Math.min(ggImage.height - height, root.xDiagram + ggImage.centerY - height/2))

                fillMode: Image.PreserveAspectFit
                smooth: true
                z: 2


                // Add smooth animations for x and y movements
                Behavior on x {
                    SmoothedAnimation {
                        easing.type: Easing.InOutQuad
                        velocity: 800
                    }
                }
                Behavior on y {
                    SmoothedAnimation {
                        easing.type: Easing.InOutQuad
                        velocity: 800
                    }
                }

            }
        }

        // Point tracking for line drawing
        Timer {
            id: pathTracker
            interval: 100  // Update every 100ms
            running: true
            repeat: true

            property var pathPoints: []

            onTriggered: {
                // Calculate the center point of the marker with limits
                const pointX = Math.max(0, Math.min(ggImage.width, pointImage.x + pointImage.width/2));
                const pointY = Math.max(0, Math.min(ggImage.height, pointImage.y + pointImage.height/2));

                // Add point to the path
                pathPoints.push(Qt.point(pointX, pointY));

                // Update the polyline path
                polyline.path = pathPoints;
            }
        }

        // Add acceleration text displays
        Row {
            anchors {
                top: ggImage.bottom
                horizontalCenter: ggImage.horizontalCenter
                topMargin: 10
            }
            spacing: 10

            // Add clear button
            Rectangle {
                id: clearButton
                width: 50
                height: 20
                color: "#636363"
                radius: 15
                border.color: "white"
                border.width: 1
                z: 3

                Text {
                    text: "Clear"
                    color: "white"
                    anchors.centerIn: parent
                    font {
                        family: "Arial"
                        pixelSize: 14
                        bold: true
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: parent.color = "#808080"
                    onExited: parent.color = "#636363"
                    onClicked: {
                        pathTracker.pathPoints = []
                        polyline.path = []
                    }
                }
            }

            Text {
                text: "Lateral G: " + communicationManager.lateralG.toFixed(2) + " G"
                color: "white"
                font {
                    family: "Arial"
                    pixelSize: 14
                    bold: true
                }
            }

            Text {
                text: "Longitudinal G: " + communicationManager.longitudinalG.toFixed(2) + " G"
                color: "white"
                font {
                    family: "Arial"
                    pixelSize: 14
                    bold: true
                }
            }
        }
    }



    /*********** Right Rect (GPS) **********/

    Rectangle {
        id : rightRect
        width : parent.width / 4
        height : parent.height - 20 * root.scaleFactor
        color : "#09122C"
        radius : 30
        border.width : 2
        border.color : "#D84040"

        anchors {
            top : statusBar.bottom
            right: parent.right
            bottom : parent.bottom
            rightMargin : 12
            topMargin : 5
            bottomMargin : 12
        }

        Text {
            id : gpsText
            text : "GPS"
            color : "turquoise"
            font {
                pixelSize: 20
                family: "DS-Digital"
            }

            anchors {
                top : parent.top
                horizontalCenter : parent.horizontalCenter
                topMargin : 5
            }
        }

        GpsPlotter {
            id : gps
            anchors {
                horizontalCenter : parent.horizontalCenter
                top : parent.top
                bottom : parent.bottom
                topMargin : 30
                bottomMargin : 10

            }
        }

    }
    /********************************************************/





}

