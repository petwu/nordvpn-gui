import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

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
                        color: '#f7f8f9'
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

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: recentClose.visible = true
                        onExited: recentClose.visible = false
                        cursorShape: Qt.PointingHandCursor
                        onClicked: MapMediator.connectToCountryById(modelData.id)
                    }

                    Button {
                        id: recentClose
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        visible: false
                        isClose: true
                        hasNoBackground: true
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
