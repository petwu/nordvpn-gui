import QtQuick 2.12
import QtQuick.Controls 2.12

import Style 1.0

import 'ui/general'
import 'ui/map'
import 'ui/sidepanel'

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
        width: sidebarToggle.hideSidebar ? 0 : 240
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

    Button {
        id: sidebarToggle
        text: '↹'
        z: 100
        width: height
        anchors.left: leftColumn.right
        anchors.bottom: leftColumn.bottom
        anchors.leftMargin: Style.marginMapButtons
        anchors.bottomMargin: Style.marginMapButtons

        property bool hideSidebar: false

        onClicked: {
            hideSidebar = !hideSidebar
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
