import QtQuick 2.12

import Style 1.0

import '../general'

Item {
    anchors.top: parent.top
    anchors.left: parent.left
    anchors.margins: Style.marginMapButtons
    width: connectionInfoText.width
    height: connectionInfoText.height
    z: 1000

    Rectangle {
        anchors.fill: parent
        color: Style.colorBase
        opacity: .6
    }

    Text {
        id: connectionInfoText
        text: 'ConnectionInfo ' + DevMediator.connectionInfo
        font.family: MonospaceFontFamily
        font.pixelSize: Style.fontSizeSmall
        padding: 8
    }

    CloseButton {
        anchors.top: parent.top
        anchors.right: parent.right
        onClicked: DevMediator.toggleDebugInformation()
    }
}
