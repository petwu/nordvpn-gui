import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import Qt.labs.settings 1.1

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

    Settings {
        id: menuSettings
        category: 'Menu'
        property alias showDebugInformation: toggleDebugInfoAction.checked
    }

    Component.onCompleted: {
        setTrayIcon()
        if (menuSettings.showDebugInformation !== DevMediator.showDebugInformation) {
            DevMediator.toggleDebugInformation()
        }
    }

    Connections {
        target: ConnectionMediator
        onIsConnectedChanged: setTrayIcon()
    }

    Connections {
        target: TrayMediator
        onOpenPreferencesWindowAction: preferencesAction.trigger()
    }

    Connections {
        target: DevMediator
        onShowDebugInformationChanged: checked => toggleDebugInfoAction.checked = checked
    }

    function setTrayIcon() {
        TrayMediator.setIconSource(ConnectionMediator.isConnected
                                   ? ':/img/tray-active'
                                   : ':/img/tray-inactive')
    }

    function openWindow(qml) {
        Qt.createComponent(qml).createObject(menuBtn).show()
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
            //: Preferences or settings where the user may configure the apps behavior.
            text: qsTr('Preferences')
            icon.name: 'settings-configure'
            action: preferencesAction
        }

        MenuSeparator { }

        MenuItem {
            //: Documentation, project website.
            text: qsTr('Help/User Guide')
            icon.name: 'dialog-question'
            action: helpAction
        }
        MenuItem {
            //: Financally support the development of this app.
            text: qsTr('Donate')
            icon.name: 'help-donate'
            action: donateAction
        }

        MenuItem {
            //: Financally support the development of this app.
            text: qsTr('Bugs/Features/Questions')
            icon.name: 'mail-send'
            action: contributingAction
        }

        MenuItem {
            //: Creadits to pay attribution to open source software used by this project.
            text: qsTr('Credits')
            icon.name: 'help-contents'
            action: creditsAction
        }

        MenuItem {
            //: About this application (version info, copyright, license, etc.).
            text: qsTr('About')
            icon.name: 'help-about'
            action: aboutAction
        }

        MenuSeparator { }

        MenuItem {
            visible: IsDebug
            //: Checkbox whether to show additional information for developers or not.
            text: qsTr('Show Debug Information')
            icon.name: 'text-x-script'
            action: toggleDebugInfoAction
        }

        MenuItem {
            //: Logout from the NordVPN service.
            text: qsTr('Logout')
            icon.name: 'system-log-out'
            action: logoutAction
        }

        MenuItem {
            //: Terminate the app (including the system tray icon).
            text: qsTr('Quit')
            icon.name: 'application-exit'
            action: quitAction
        }
    }

    Action {
        id: preferencesAction
        onTriggered: openWindow('qrc:/ui/windows/PreferencesWindow.qml')
    }

    Action {
        id: helpAction
        onTriggered: Qt.openUrlExternally(DocsURL)
    }

    Action {
        id: donateAction
        onTriggered: Qt.openUrlExternally(DonateURL)
    }

    Action {
        id: contributingAction
        onTriggered: Qt.openUrlExternally(IssuesURL)
    }

    Action {
        id: creditsAction
        onTriggered: openWindow('qrc:/ui/windows/CreditsWindow.qml')
    }

    Action {
        id: aboutAction
        onTriggered: openWindow('qrc:/ui/windows/AboutWindow.qml')
    }

    Action {
        id: toggleDebugInfoAction
        checkable: true
        onTriggered: DevMediator.toggleDebugInformation()
        onCheckedChanged: {
            menuSettings.setValue('showDebugInformation', checked)
        }
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
