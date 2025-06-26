import QtQuick 2.15
import QtQuick.Controls
import "../.."

Rectangle {
    id: root

    property real scaleFactor : 1.0
    color: "#1A3438"
    anchors.fill: parent
    radius: 40
    border.color: "#A6F1E0"
    border.width: 5

    /**** Layout for the Two Rectangles ***/

    Row {
        anchors.fill : parent
        anchors.margins : 40 * root.scaleFactor
        spacing : 10


        /**** Left Rectangle ****/

        Rectangle {
            id : leftRect
            width : parent.width / 3
            height : parent.height - 10 * root.scaleFactor
            color : "#09122C"
            radius : 30
            border.width : 1.5
            border.color : "#D84040"


            /**** Image of the Formula Student Car ****/


            Image {
                id : carImg
                source : "../Assets/CAR-215-ASURT.png"
                width : 350 * root.scaleFactor
                height : 300 * root.scaleFactor
                fillMode : Image.PreserveAspectFit
                smooth : true

                anchors {
                    top : formulalogo.top
                    horizontalCenter : parent.horizontalCenter
                    topMargin : 40
                }
            }


            /**** Formula logo ****/


            Image {
                id : formulalogo
                source : "../Assets/formulalogo.jpeg"
                width : 200
                height : 100
                fillMode : Image.PreserveAspectFit
                smooth : true
                anchors {
                    top : parent.top
                    left : parent.left
                    leftMargin : 10
                }
            }


            /**** Layout for the Components of this Rectangle ****/

            Column {
                spacing : 8

                anchors {
                    horizontalCenter : parent.horizontalCenter
                    top : carImg.bottom
                    topMargin : 10
                }

                /**** Field to Get Session Name ****/

                Text {
                    id : sessionInfo
                    text : "Please Enter Session details"
                    font {
                        family : "DS-Digital"
                        pixelSize : 18 * root.scaleFactor
                        bold : true
                    }

                    anchors.horizontalCenter : parent.horizontalCenter
                    color: "turquoise"
                }

                Text {
                    id : sessionName
                    text : "Enter Session Name : "
                    font {
                        pixelSize : 14 * root.scaleFactor
                        bold : true
                    }

                    anchors.left : parent.left
                    color : "white"
                }

                TextField {
                    id : sessionNameField
                    placeholderText: startButton.inValid_Name ? "Don't Leave This Empty!" : "Enter Session Name"
                    placeholderTextColor: startButton.inValid_Name ? "darkRed" : "turquoise"

                    width: 300 * root.scaleFactor
                    height: 30 * root.scaleFactor

                    font.pointSize : 12 * root.scaleFactor
                    verticalAlignment :  TextInput.AlignVCenter
                    anchors.horizontalCenter : parent.horizontalCenter
                    color : "white"

                    background : Rectangle {
                        color: "#636363"
                        radius: 50
                        border.color: parent.activeFocus ? "turquoise" : startButton.inValid_Name ? "darkRed" : "transparent"
                        border.width : 4
                    }

                }


                /**** Field to Get Port Number ****/


                Text {
                    text: "Choose Port:"
                    font {
                        bold: true
                        pixelSize: 14 * root.scaleFactor
                    }
                    anchors {
                        left: parent.left
                    }
                    color: "white"
                }

                TextField {
                    id : serverClientPortField
                    visible : server_clientradio.checked
                    placeholderText: startButton.inValid_Port ? "Don't Leave This Empty!" : "Enter Port (e.g. , 8080)"
                    placeholderTextColor: startButton.inValid_Port ? "darkRed" : "turquoise"

                    width: 300 * root.scaleFactor
                    height: 30 * root.scaleFactor

                    font.pointSize : 12 * root.scaleFactor
                    verticalAlignment :  TextInput.AlignVCenter
                    anchors.horizontalCenter : parent.horizontalCenter
                    color : "white"

                    background : Rectangle {
                        color: "#636363"
                        radius: 50
                        border.color: parent.activeFocus ? "turquoise" : startButton.inValid_Port ? "darkRed" : "transparent"
                        border.width : 4
                    }
                }

                ComboBox {
                    id : serialPortField
                    visible : serialradio.checked
                    model: ["COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "COM8", "COM9" , "COM10" , "COM11" , "COM12"]
                    currentIndex: 2
                    width: 300 * root.scaleFactor
                    height: 30 * root.scaleFactor

                    font.pointSize : 12 * root.scaleFactor
                    anchors.horizontalCenter : parent.horizontalCenter

                    contentItem: Text {
                        text: serialPortField.displayText
                        color: "turquoise"
                        font.pointSize: 13 * root.scaleFactor
                        verticalAlignment: Text.AlignVCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 20
                    }

                    background: Rectangle {
                        color: "#636363"
                        radius: 100
                        border.color: parent.activeFocus ? "turquoise" : "transparent"
                        border.width: 4
                    }

                    delegate : ItemDelegate {
                        background: Rectangle {
                            id: itemBackground
                            width: 300 * root.scaleFactor
                            color: "#636363"
                        }

                        contentItem: Text {
                            text: modelData
                            color: "turquoise"
                            font.pointSize: 11 * root.scaleFactor
                        }

                        MouseArea {
                            id: control
                            anchors.fill: itemBackground
                            hoverEnabled: true
                            onEntered: {
                                itemBackground.color = "#4a4848" // Color when hovered
                            }

                            onPressed: {
                                serialPortField.currentIndex = index
                                serialPortField.popup.close()
                            }
                            onExited: {
                                itemBackground.color = "#636363" // Default color when not hovered
                            }
                        }
                    }
                }

                /**** Field to Get Baud Rate ****/

                Text {
                    visible : serialradio.checked
                    text: "Select Baud Rate:"
                    font {
                        bold: true
                        pixelSize: 14 * root.scaleFactor
                    }
                    anchors {
                        left: parent.left
                    }
                    color: "white"
                }

                ComboBox {
                    id : serialBaudRate
                    visible : serialradio.checked
                    model: ["9600", "57600", "115200"]
                    currentIndex: 2
                    width: 300 * root.scaleFactor
                    height: 30 * root.scaleFactor

                    font.pointSize : 12 * root.scaleFactor
                    anchors.horizontalCenter : parent.horizontalCenter

                    contentItem: Text {
                        text: serialBaudRate.displayText
                        color: "turquoise"
                        font.pointSize: 13 * root.scaleFactor
                        verticalAlignment: Text.AlignVCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 20
                    }

                    background: Rectangle {
                        color: "#636363"
                        radius: 100
                        border.color: parent.activeFocus ? "turquoise" : "transparent"
                        border.width: 4
                    }

                    delegate : ItemDelegate {
                        background: Rectangle {
                            id: itemBackground1
                            width: 300 * root.scaleFactor
                            color: "#636363"
                        }

                        contentItem: Text {
                            text: modelData
                            color: "turquoise"
                            font.pointSize: 11 * root.scaleFactor
                        }

                        MouseArea {
                            anchors.fill: itemBackground1
                            hoverEnabled: true
                            onEntered: {
                                itemBackground1.color = "#4a4848" // Color when hovered
                            }

                            onPressed: {
                                serialBaudRate.currentIndex = index
                                serialBaudRate.popup.close()
                            }
                            onExited: {
                                itemBackground1.color = "#636363" // Default color when not hovered
                            }
                        }
                    }
                }


            }


            /**** Start Button ****/


            MyButton {
                id : startButton
                property bool inValid_Name : false
                property bool inValid_Port : false

                source : "../Assets/powerButton.png"
                hoverText : "Start Session"
                width : 50 * root.scaleFactor
                height : 50 * root.scaleFactor
                fillMode : Image.PreserveAspectFit
                smooth : true

                anchors {
                    horizontalCenter : parent.horizontalCenter
                    bottom : parent.bottom
                    bottomMargin : 50
                }

                onClicked : {
                    inValid_Name = (sessionNameField.text === "")

                    if (server_clientradio.checked) {
                        inValid_Port = (serverClientPortField.text === "")
                        if(!inValid_Name && !inValid_Port) {
                            var portNumber = parseInt(serverClientPortField.text.trim())
                            if (isNaN(portNumber) || portNumber < 1 || portNumber > 65535) {
                                inValid_Port = true
                                return
                            }
                            var success = communicationManager.startUdp(portNumber)
                            if (success) {
                                console.log("UDP client started successfully on port: " + portNumber)
                                stackView.push("WaitingScreen.qml", {
                                    "sessionName": sessionNameField.text,
                                    "portNumber": portNumber,
                                    "isSerialSource": false
                                })
                            } else {
                                console.error("Failed to start UDP client on port: " + portNumber)
                                inValid_Port = true
                            }
                        }
                    } else if (serialradio.checked) {
                        inValid_Port = (serialPortField.currentIndex === -1 || serialBaudRate.currentIndex === -1)
                        if(!inValid_Name && !inValid_Port) {
                            var portName = serialPortField.model[serialPortField.currentIndex]
                            var baudRate = parseInt(serialBaudRate.model[serialBaudRate.currentIndex])
                            var success = communicationManager.startSerial(portName, baudRate)
                            if (success) {
                                console.log("Serial manager started successfully on port: " + portName + " with baud rate: " + baudRate)
                                stackView.push("WaitingScreen.qml", {
                                    "sessionName": sessionNameField.text,
                                    "portName": portName,
                                    "baudRate": baudRate,
                                    "isSerialSource": true
                                })
                            } else {
                                console.error("Failed to start Serial manager on port: " + portName)
                                inValid_Port = true
                            }
                        }
                    }
                }
            }

            Text {
                id : startText
                text : "Start"

                font {
                    family : "DS-Digital"
                    bold : true
                    pixelSize : 20 * root.scaleFactor
                }
                color: "turquoise"
                anchors {
                    horizontalCenter : parent.horizontalCenter
                    top : startButton.bottom
                    topMargin : 5
                }
            }
        }


        /**** Right Rectangle (Extra Information Rectangle)  ****/

        Rectangle {
            id : extraInfromationRect
            width : (2 * parent.width / 3) - 50
            height : parent.height - 10 * root.scaleFactor
            color : "#09122C"
            radius : 30
            border.width : 1.5
            border.color : "#D84040"


            /**** Road Image ****/

            Image {
                id : roadImg
                source : "../Assets/road2.png"
                width : 400 * root.scaleFactor
                height : 400 * root.scaleFactor
                fillMode : Image.PreserveAspectFit
                smooth : true
                anchors {
                    right : parent.right
                    verticalCenter : parent.verticalCenter
                }
            }

            /**** Car on The Road ****/

            Image {
                id : roadCarImg
                source : "../Assets/car3_white.png"
                width: 150 * root.scaleFactor
                height: 150 * root.scaleFactor
                fillMode: Image.PreserveAspectFit
                smooth: true
                anchors.centerIn: roadImg
            }


            /**** Team Logo ****/

            Image  {
                id : racingTeamLogo
                source : "../Assets/racinglogo.png"
                width : 100 * root.scaleFactor
                height : 100 * root.scaleFactor
                fillMode : Image.PreserveAspectFit
                smooth : true

                anchors {
                    right : parent.right
                    top : parent.top

                    topMargin : 10
                    rightMargin : 10
                }
            }

            /**** Layout for the Fields ****/


            Column {
                id :col
                anchors {
                    left : parent.left
                    top : parent.top
                    margins : 20
                }
                spacing : 15

                /**** Field to Get Driver Name ****/

                Text {
                    id : extraText
                    text : "Extra Information"
                    font {
                        bold : true
                        pixelSize : 16 * root.scaleFactor
                        family : "Amiri"
                    }
                    color: "turquoise"
                }
                Text {
                    text: "Enter Driver Name:"
                    font {
                        bold: true
                        pixelSize: 13 * root.scaleFactor
                        family : "Amiri"
                    }
                    anchors {
                        left: parent.left
                    }
                    color: "white"
                }
                TextField {
                    id : driverNameField
                    placeholderText : "Enter Driver Name"
                    placeholderTextColor : "turquoise"
                    width : 300 * root.scaleFactor
                    height : 30 * root.scaleFactor

                    font.pointSize : 12 * root.scaleFactor
                    verticalAlignment : TextInput.AlignVCenter
                    anchors.left : parent.left
                    color: "white"
                    background: Rectangle {
                        color: "#636363"
                        radius: 100
                        border.color: parent.activeFocus ? "turquoise" : "transparent"
                         border.width: 4
                    }

                }


                /**** Area to Write the Session Goals ****/

                Text {
                    text: "Enter Session Goals:"
                    font {
                        bold: true
                        pixelSize: 13 * root.scaleFactor
                        family : "Amiri"
                    }
                    anchors {
                        left: parent.left
                    }
                    color: "white"
                }
                TextArea {
                    id: goalsField
                    width: 350 * root.scaleFactor
                    height: 250 * root.scaleFactor
                    wrapMode: Text.Wrap
                    placeholderText: "Write your goals here..."
                    placeholderTextColor: "turquoise"
                    font.pointSize: 13 * root.scaleFactor
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: "white"
                    background: Rectangle {
                        color: "#636363"
                        radius: 10
                        border.color: parent.activeFocus ? "turquoise" : "transparent"
                        border.width: 4
                    }
                }


                Text {
                    text : "Choose type of Communication : "
                    font {
                        bold : true
                        pixelSize : 15 * root.scaleFactor
                        family : "Amiri"
                    }

                    anchors.left : parent.left
                    color : "turquoise"
                }

                RadioButton {
                    id : server_clientradio
                    checked : true
                    Text {
                        id : radio1text
                        text : "Server Client Communication"
                        font {
                            bold : true
                            family : "DS-Digital"
                            pixelSize : 18 * root.scaleFactor
                        }
                        color: "white"

                        anchors.left : parent.right
                        anchors.leftMargin : 5

                    }
                }

                RadioButton {
                    id : serialradio
                    Text {
                        id : radio2text
                        text : "Serial Communication"
                        font {
                            bold : true
                            family : "DS-Digital"
                            pixelSize : 18 * root.scaleFactor
                        }
                        color: "white"

                        anchors.left : parent.right
                        anchors.leftMargin : 5

                    }
                }
            }

        }

    }
}
