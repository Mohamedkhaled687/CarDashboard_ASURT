import QtQuick 2.15

Item {
    id: root
    property int batteryLevel: 0
    property real scaleFactor: 1.0

    Column {
        id: indicator
        anchors.centerIn: parent
        spacing: 2 * root.scaleFactor
        Image {
            id: batteryIcon
            source: (root.batteryLevel > 30) ? "../Assets/batteryIcon_blue.png" : "../Assets/batteryIcon.png"
            width: 30
            height: 30
            fillMode: Image.PreserveAspectFit
            smooth: true
        }
        Text {
            id: batteryText
            text: root.batteryLevel + "%"
            color: batteryIndicator.getColorForBattery(root.batteryLevel)
            font {
                pixelSize: 20 * root.scaleFactor
                bold: true
                family: "DS-Digital"
            }
        }
        Item {
            id: batteryIndicator
            width: 200 * root.scaleFactor
            height: 100 * root.scaleFactor
            function getColorForBattery(level) {
                var r = Math.floor(255 * (1 - level / 100));
                var g = Math.floor(255 * (level / 100));
                var b = 0;
                return "#" + ((1 << 24) + (r << 16) + (g << 8) + b).toString(16).slice(1).toUpperCase();
            }
            Canvas {
                id: batteryCanvas
                anchors.fill: parent
                anchors.centerIn: parent
                onPaint: {
                    var ctx = getContext("2d");
                    var width = batteryCanvas.width / 5;
                    var height = batteryCanvas.height;
                    var barHeight = (height / 10);
                    var numberOfBars = 10;
                    ctx.clearRect(0, 0, width, height);
                    var barColor = batteryIndicator.getColorForBattery(root.batteryLevel);
                    for (var i = 0; i < numberOfBars; i++) {
                        var yPosition = i * barHeight;
                        ctx.fillStyle = (i < Math.ceil((100 - root.batteryLevel) / 10)) ? "black" : barColor;
                        ctx.fillRect(0, yPosition, width, barHeight - 2);
                    }
                }
            }
        }
        Connections {
            target: communicationManager
            onBatteryLevelChanged: {
                batteryLevel = communicationManager ? communicationManager.batteryLevel : 0;
                batteryCanvas.requestPaint();
            }
        }
    }
}
