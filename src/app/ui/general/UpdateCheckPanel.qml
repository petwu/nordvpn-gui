import QtQuick 2.12

import Style 1.0

import '.'

Button {
    anchors.margins: Style.marginMapButtons
    visible: UpdateMediator.showUpdatePanel
    z: 1000
    text: qsTr('Update available')
    icon.name: 'dialog-information'
    icon.color: Style.colorSuccess
    onClicked: UpdateMediator.showUpdateDialog()
}
