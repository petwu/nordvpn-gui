import QtQuick 2.12
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3

import "ui/map"

ApplicationWindow {
    id: window
    // TODO: remove
    x: 920
    y: 0

    visible: true
    title: "NordVPN"
    minimumWidth: 720
    minimumHeight: 400
    width: 1000
    height: 600

    Column {
        id: leftColumn
        width: 240
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        Button {
            objectName: "btn"
            text: "Click Me."
        }

        Text {
            text: 'connecting = ' + MapMediator.connectingCountryId +
                  '\nconnected = ' + MapMediator.connectedCountryId +
                  '\nconnect-commands paused = ' + MapMediator.areConnectionCommandsPaused
        }
    }

    Rectangle {
        id: rightColumn
        color: "#b8cedc"
        anchors.top: parent.top
        anchors.left: leftColumn.right
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        clip: true

        ColumnLayout {
            width: parent.width
            height: parent.height

            Map {
                id:map

                ConnectionStatusPanel {
                    id: statusPanel
                    anchors.horizontalCenter: map.horizontalCenter
                    anchors.bottom: map.bottom
                    anchors.bottomMargin: 32
                }
            }
        }
    }
}
