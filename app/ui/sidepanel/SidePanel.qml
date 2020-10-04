import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import Style 1.0

import '.'
import '../general'
import '../icons'

ScrollView {
    id: sidePanel

    Column {
        width: parent.width

        Collapsible {
            title: 'Recents'
            spacing: 0

            Repeater {
                model: SidePanelMediator.recentCountries

                Item {
                    id: recentItem
                    Layout.margins: 4
                    width: sidePanel.width
                    height: 32

                    Rectangle {
                        anchors.fill: parent
                        visible: recentClose.visible
                        color: Style.colorBase
                    }

                    IconClock {
                        id: iconClock
                        x: 7
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Text {
                        anchors.left: iconClock.right
                        anchors.leftMargin: 8
                        anchors.verticalCenter: parent.verticalCenter
                        text: modelData.name
                    }

                    HoverHandler {
                        onHoveredChanged: recentClose.visible = hovered
                    }

                    CloseButton {
                        id: recentClose
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        visible: false
                        onClicked: SidePanelMediator.removeFromRecentsList(modelData.id)
                    }
                }
            }
        }

        Collapsible {
            title: 'Specialty Servers'
        }

        Collapsible {
            title: 'Countries'
        }

        Collapsible {
            title: 'Regions'
        }
    }
}
