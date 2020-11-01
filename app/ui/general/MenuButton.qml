import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2

import Style 1.0

import '.'

Item {
    id: menuBtn
    z: 1000
    anchors.top: parent.top
    anchors.right: parent.right
    anchors.margins: Style.marginMapButtons
    width: btn.width
    height: btn.height

    Component.onCompleted: setTrayIcon()

    Connections {
        target: Mediator
        onIsConnectedChanged: setTrayIcon()
    }

    Connections {
        target: TrayMediator
        onOpenPreferencesWindowAction: preferencesAction.trigger()
    }

    function setTrayIcon() {
        TrayMediator.setIconSource(Mediator.isConnected
                                   ? ':/img/tray-active'
                                   : ':/img/tray-inactive')
    }

    Button {
        id: btn
        anchors.centerIn: parent
        width: height
        icon.name: 'preferences-other'
        display: AbstractButton.IconOnly
        onClicked: menu.open()
    }

    Menu {
        id: menu
        x: -(width - btn.width)
        y: btn.height + 4

        MenuItem {
            text: 'Preferences'
            icon.name: 'settings-configure'
            action: preferencesAction
        }

        MenuItem {
            text: 'Logout'
            icon.name: 'system-log-out'
            action: logoutAction
        }

        MenuItem {
            text: 'Quit'
            icon.name: 'application-exit'
            action: quitAction
        }
    }

    Action {
        id: preferencesAction
        onTriggered: Qt.createComponent('qrc:/ui/windows/PreferencesWindow.qml').createObject(menuBtn).show()
    }

    Action {
        id: logoutAction
        onTriggered: AccountMediator.logout()
    }

    Action {
        id: quitAction
        onTriggered: Qt.quit()
    }
}
