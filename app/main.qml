import QtQuick 2.12
import QtQuick.Controls 2.1

import "ui/map"
import "ui/sidepanel"

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

    SidePanel {
        id: leftColumn
        width: hideSidebarBtn.hidden ? 0 : 240
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.bottom: parent.bottom

        Behavior on width {
            NumberAnimation {
                duration: 100
                easing.type: Easing.InOutQuad
            }
        }
    }

    MapButton {
        id: hideSidebarBtn
        anchors.left: leftColumn.right
        anchors.bottom: leftColumn.bottom
        text: '↹'

        property bool hidden: false

        onClicked: {
            hidden = !hidden
        }
    }

    Map {
        id: rightColumn
        anchors.top: parent.top
        anchors.left: leftColumn.right
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        ConnectionStatusPanel {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 32
        }
    }
}
