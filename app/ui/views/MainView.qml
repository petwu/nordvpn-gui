import QtQuick 2.12

import Style 1.0

import '../general'
import '../map'
import '../sidepanel'

Item {
    MenuButton {
    }

    SidePanel {
        id: leftColumn
        width: sidebarToggle.hideSidebar ? 0 : 250
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        z: 1000
        clip: sidebarToggle.hideSidebar

        Behavior on width {
            NumberAnimation {
                duration: 100
                easing.type: Easing.InOutQuad
            }
        }

        Behavior on clip {
            NumberAnimation {
                duration: 100
                easing.type: Easing.InOutQuad
            }
        }
    }

    Rectangle {
        z: 1000
        width: 1
        height: window.height
        anchors.top: leftColumn.top
        anchors.right: leftColumn.right
        color: Style.colorLight
    }

    Button {
        id: sidebarToggle
        text: /* no qsTr() */ '↹'
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
