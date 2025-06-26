import QtQuick 2.15
import QtQuick.Controls 2.15
import QtLocation
import QtPositioning

Rectangle {
    id: gpsDisplay
    width: parent.width - 30
    height: parent.height - 50

    // These properties hold the raw GPS values from udpClient
    property real gpsLongitude: 0.0
    property real gpsLatitude: 0.0
    property real currentLongitude: gpsLongitude
    property real currentLatitude: gpsLatitude

    // Add a property list to store path coordinates
    property var pathCoordinates: []
    property bool firstCoordinateReceived: false

    // Add error handling for invalid coordinates
    function isValidCoordinate(lat, lon) {
        return !isNaN(lat) && !isNaN(lon) &&
               lat >= -90 && lat <= 90 &&
               lon >= -180 && lon <= 180;
    }

    // Modified function to update path
    function updatePath(lat, lon) {
        var newCoord = QtPositioning.coordinate(lat, lon);

        if (!firstCoordinateReceived) {
            firstCoordinateReceived = true;
            pathCoordinates = [newCoord];
        } else {
            // Create a new array with all existing coordinates plus the new one
            var newPath = pathCoordinates.slice(); // Create a copy of the existing array
            newPath.push(newCoord);
            pathCoordinates = newPath; // Assign the new array to trigger update
        }
        // Force update of the polyline
        pathLine.path = pathCoordinates;
    }

    Map {
        id: map
        anchors.fill: parent
        zoomLevel: 14
        center: QtPositioning.coordinate(0, 0)
        plugin: Plugin {
            name: "osm"
            PluginParameter {
                name: "osm.useragent"
                value: "MyGPSApp"
            }
            PluginParameter {
                name: "osm.mapping.custom.host"
                value: "https://tile.openstreetmap.org/"
            }
        }



        // Modified MapPolyline for better visibility
        MapPolyline {
            id: pathLine
            line.width: 4
            line.color: "blue"
            path: gpsDisplay.pathCoordinates
        }

        MapQuickItem {
            id: carMarker
            coordinate: QtPositioning.coordinate(gpsDisplay.currentLatitude, gpsDisplay.currentLongitude)
            anchorPoint.x: marker.width / 2
            anchorPoint.y: marker.height
            sourceItem: Image {
                id: marker
                width: 20
                height: 20
                source: "../Assets/marker.png"
                z : 2
            }
        }
    }

    Row {
        spacing: 10
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.topMargin: 10
        anchors.rightMargin: 10
        Button {
            text: "+"
            width: 20
            height: 20
            onClicked: map.zoomLevel = Math.min(map.zoomLevel + 1, 20)
        }
        Button {
            text: "-"
            width: 20
            height: 20
            onClicked: map.zoomLevel = Math.max(map.zoomLevel - 1, 2)
        }
        Button {
            text: "Clear"
            width: 50
            height: 20
            onClicked: {
                gpsDisplay.pathCoordinates = [];
                gpsDisplay.firstCoordinateReceived = false;
                pathLine.path = [];
            }
        }
    }

    Behavior on currentLongitude {
        NumberAnimation { duration: 25; easing.type: Easing.InOutQuad }
    }
    Behavior on currentLatitude {
        NumberAnimation { duration: 25; easing.type: Easing.InOutQuad }
    }

    Connections {
        target: communicationManager
        function onGpsLongitudeChanged() {
            if (isValidCoordinate(communicationManager.gpsLatitude , communicationManager.gpsLongitude)) {
                gpsDisplay.gpsLongitude = communicationManager.gpsLongitude;
                currentLongitude = communicationManager.gpsLongitude;
                currentLatitude = communicationManager.gpsLatitude;
                var newCoord = QtPositioning.coordinate(communicationManager.gpsLatitude, communicationManager.gpsLongitude);
                map.center = newCoord;
                // Only update path when both coordinates have changed
                updatePath(communicationManager.gpsLatitude, communicationManager.gpsLongitude);
            }
        }
        function onGpsLatitudeChanged() {
            if (isValidCoordinate(communicationManager.gpsLatitude , communicationManager.gpsLongitude)) {
                gpsDisplay.gpsLatitude = communicationManager.gpsLatitude;
                currentLongitude = communicationManager.gpsLongitude;
                currentLatitude = communicationManager.gpsLatitude;
                var newCoord = QtPositioning.coordinate(communicationManager.gpsLatitude, communicationManager.gpsLongitude);
                map.center = newCoord;
                // Only update path when both coordinates have changed
                updatePath(communicationManager.gpsLatitude, communicationManager.gpsLongitude);
            }
        }
    }
}
