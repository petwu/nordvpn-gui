import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.12

import "../general" as MyControls
import "."

import Style 1.0

Item {
    id: statusPanel
    width: bkgd.width
    height: bkgd.height
    z: MapMediator.countryList.length + 100

    Connections {
        target: MapMediator
        onIsRatingPossibleChanged: (possible) => _.showRating = possible
        onDisconnectedChanged: (disconnected) => disconnected ? btnQuickConnect.enabled = true : null
        onConnectedChanged: (connected) => connected ? btnDisconnect.enabled = true : null
    }

    QtObject {
        id: _
        property bool showRating: false
    }

    MouseArea {
        anchors.fill: bkgd
        hoverEnabled: true
    }

    Rectangle {
        id: bkgd
        color: Style.colorStatusPanelBkgd
        width: content.width + 24
        height: content.height + 24

        RowLayout {
            x: 12
            y: 12

            RowLayout {
                id: content
                Layout.minimumWidth: 440

                Column {
                    Layout.alignment: Qt.AlignBottom

                    RowLayout {
                        visible: !_.showRating
                        spacing: 6

                        Rectangle {
                            width: 6
                            height: width
                            radius: width/2
                            Layout.alignment: Qt.AlignVCenter
                            color: {
                                if (MapMediator.isDisconnected) return Style.colorStatusPanelDisconnected
                                if (MapMediator.isConnecting)   return Style.colorStatusPanelConnecting
                                if (MapMediator.isConnected)    return Style.colorStatusPanelConnected
                                return 'transparent'
                            }
                        }

                        Text {
                            text: {
                                if (MapMediator.isDisconnected) return 'Unprotected'
                                if (MapMediator.isConnecting)   return 'Connecting'
                                if (MapMediator.isConnected)    return 'Protected'
                                return ''
                            }
                            color: {
                                if (MapMediator.isDisconnected) return Style.colorStatusPanelDisconnected
                                if (MapMediator.isConnecting)   return Style.colorStatusPanelConnecting
                                if (MapMediator.isConnected)    return Style.colorStatusPanelConnected
                                return 'transparent'
                            }
                            font.pixelSize: .8*Qt.application.font.pixelSize
                            font.capitalization: Font.AllUppercase
                        }
                    }

                    RowLayout {
                        Text {
                            text: {
                                const country = MapMediator.countryList.find((c) => {
                                                                                 return c.id === MapMediator.connectedCountryId
                                                                             })
                                if (_.showRating)                       return 'Rate your connection speed'
                                if (MapMediator.isDisconnected)         return 'Pick country or use quick connect'
                                if (MapMediator.isConnecting)           return 'Finding the best server ...'
                                if (MapMediator.isConnected && country) return 'Connected to ' + country.statusName + ' #' + MapMediator.connectedServerId
                                return ''
                            }
                        }

                        Text {
                            text: MapMediator.isConnected && MapMediator.connectedIP
                                  ? '(' + MapMediator.connectedIP + ')'
                                  : ''
                            font.pixelSize: .9*Qt.application.font.pixelSize
                            color: Style.colorStatusPanelSeconary
                            font.weight: Font.Thin
                        }
                    }

                    Text {
                        visible: _.showRating
                        text: 'Your feedback helps us improve!'
                        font.pixelSize: .8*Qt.application.font.pixelSize
                        color: Style.colorStatusPanelSeconary
                    }
                }

                Item {
                    Layout.fillWidth: true
                }

                MyControls.Button {
                    id: btnQuickConnect
                    visible: MapMediator.isDisconnected && !_.showRating
                    text: 'Quick Connect'
                    onClicked: {
                        enabled = false
                        MapMediator.quickConnect()
                    }
                }

                MyControls.Button {
                    id: btnDisconnect
                    visible: MapMediator.isConnected
                    text: 'Disconnect'
                    isInverted: true
                    isDanger: true
                    onClicked: {
                        enabled = false
                        MapMediator.disconnect()
                    }
                }

                RowLayout {
                    visible: _.showRating
                    height: parent.height

                    RatingStars {
                        onRated: _.showRating = false
                    }

                    Item {
                        width: 4
                    }

                    Rectangle {
                        width: 2
                        height: 32
                        color: Style.colorLightGray
                    }

                    MyControls.Button {
                        isClose: true
                        isInverted: true
                        hasNoBackground: true
                        onClicked: {
                            _.showRating = false
                        }
                    }
                }
            }
        }
    }

    DropShadow {
        anchors.fill: bkgd
        source: bkgd
        horizontalOffset: 0
        verticalOffset: 1.25
        radius: 6
        samples: 13
        color: Style.colorStatusPanelShadow
    }
}
