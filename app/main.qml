import QtQuick 2.12
import QtQuick.Controls 2.12
//import Qt.labs.platform 1.1

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

    //onClosing: window.hide()

    QtObject {
        id: _
        property bool preventQuit: true
    }

    // TODO: use QSystemTrayIcon (C++), since Qt Labs Platform is experimental and has bugs, e.g.
    // it's not possible to show a icon next to a MenuItem's text (https://bugreports.qt.io/browse/QTBUG-84102)
    /*
    SystemTrayIcon {
        id: systemTray
        visible: true
        icon.source: MapMediator.isConnected
                     ? 'qrc:/img/tray-active'
                     : 'qrc:/img/tray-inactive'
        menu: Menu {
            MenuItem {
                text: 'Show Window'
                onTriggered: {
                    window.show()
                    window.raise()
                    window.requestActivate()
                }
            }

            MenuItem {
                text: 'Quit'
                onTriggered: Qt.quit()
            }
        }
        onActivated: (reason) => {
                         if (reason === SystemTrayIcon.Trigger ||
                             reason === SystemTrayIcon.MiddleClick) {
                             if (window.visible) {
                                 window.hide()
                             } else {
                                 window.show()
                                 window.raise()
                                 window.requestActivate()
                             }
                         }
                     }
    }
    */

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
