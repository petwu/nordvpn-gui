import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.12

import Style 1.0

import '.'
import '../general'
import '../icons'

ScrollView {
    id: scrollView
    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

    property int rowHeigth: 40
    property int iconTextSpacing: 8
    property int iconSize: 20
    property int minContentWidth: 100

    onAvailableWidthChanged: {
        if (availableWidth > minContentWidth) {
            sidePanel.width = availableWidth
        }
    }

    Column {
        id: sidePanel
        width: minContentWidth

        Collapsible {
            id: sectionRecents
            title: 'Recents'
            visible: SidePanelMediator.recentCountries.length > 0
            spacing: 0

            Repeater {
                model: SidePanelMediator.recentCountries

                Item {
                    width: sidePanel.width
                    height: rowHeigth

                    Rectangle {
                        anchors.fill: parent
                        visible: hoverRecent.hovered
                        color: Style.colorBase
                    }

                    IconClock {
                        size: iconSize
                        id: iconClock
                        x: sectionRecents.headerPadding+sectionRecents.handleSize/2-width/2
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Text {
                        anchors.left: iconClock.right
                        anchors.leftMargin: iconTextSpacing
                        anchors.verticalCenter: parent.verticalCenter
                        text: modelData.name
                    }

                    HoverHandler {
                        id: hoverRecent
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: MapMediator.areConnectionCommandsPaused
                                     ? Qt.ArrowCursor
                                     : Qt.PointingHandCursor
                        onClicked: MapMediator.connectToCountryById(modelData.id)
                    }

                    CloseButton {
                        id: recentClose
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        visible: hoverRecent.hovered
                        onClicked: SidePanelMediator.removeFromRecentsList(modelData.id)
                    }
                }
            }
        }

        Collapsible {
            title: 'Specialty Servers'
        }

        Collapsible {
            id: sectionCountries
            title: 'Countries'
            spacing: 0

            Repeater {
                model: MapMediator.countryList

                Item {
                    width: sidePanel.width
                    height: rowHeigth

                    Rectangle {
                        anchors.fill: parent
                        visible: hoverCountry.hovered
                        color: Style.colorBase
                    }

                    Rectangle {
                        id: flag
                        x: sectionCountries.headerPadding+sectionCountries.handleSize/2-width/2
                        width: iconSize
                        height: width
                        anchors.verticalCenter: parent.verticalCenter
                        color: 'transparent'

                        Image {
                            id: flagImg
                            source: 'qrc:/flag/'+modelData.countryCode
                            sourceSize.height: parent.height
                            anchors.centerIn: parent
                            fillMode: Image.PreserveAspectFit
                            smooth: true
                            visible: false
                        }

                        OpacityMask {
                            anchors.fill: parent
                            source: flagImg
                            maskSource: Rectangle {
                                width: flag.width
                                height: flag.height
                                radius: width
                            }
                        }

                        Rectangle {
                            anchors.fill: flag
                            radius: width
                            color: '#ffffff'
                            opacity: MapMediator.areConnectionCommandsPaused
                                     ? .33
                                     : 0
                        }
                    }

                    Text {
                        text: modelData.name
                        anchors.left: flag.right
                        anchors.leftMargin: iconTextSpacing
                        anchors.verticalCenter: parent.verticalCenter
                        color: modelData.id === MapMediator.connectedCountryId
                               ? Style.colorGreen
                               : (modelData.id === MapMediator.connectingCountryId
                                  ? Style.colorOrange
                                  : Style.colorText)
                    }

                    HoverHandler {
                        id: hoverCountry
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: MapMediator.areConnectionCommandsPaused
                                     ? Qt.ArrowCursor
                                     : Qt.PointingHandCursor
                        onClicked: MapMediator.connectToCountryById(modelData.id)
                    }
                }
            }
        }
    }
}
