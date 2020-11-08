import QtQuick 2.12

import Style 1.0

import '../general'
import '../icons'

Item {
    id: recent

    property int countryId: -1
    property string countryName: ''
    property int iconTextSpacing: 8
    property int iconSize: 20
    property double iconHorizontalCenter: 0

    Rectangle {
        anchors.fill: parent
        visible: hoverRecent.hovered
        color: Style.colorBase
    }

    IconClock {
        id: iconClock
        size: recent.iconSize
        x: recent.iconHorizontalCenter-width/2
        anchors.verticalCenter: parent.verticalCenter
    }

    Text {
        anchors.left: iconClock.right
        anchors.leftMargin: recent.iconTextSpacing
        anchors.verticalCenter: parent.verticalCenter
        text: qsTranslate('Country', recent.countryName)
    }

    HoverHandler {
        id: hoverRecent
    }

    MouseArea {
        anchors.fill: parent
        cursorShape: Mediator.areConnectionCommandsPaused
                     ? Qt.ArrowCursor
                     : Qt.PointingHandCursor
        onClicked: Mediator.connectToCountryById(recent.countryId)
    }

    CloseButton {
        anchors.right: parent.right
        anchors.margins: (parent.height-height)/2
        anchors.verticalCenter: parent.verticalCenter
        visible: hoverRecent.hovered
        onClicked: Mediator.removeFromRecentsList(recent.countryId)
    }
}
