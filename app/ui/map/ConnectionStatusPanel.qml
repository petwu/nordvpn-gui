import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.12

import '../general'
import '.'

import Style 1.0

Item {
    id: statusPanel
    width: bkgd.width
    height: bkgd.height
    z: 100

    Connections {
        target: Mediator
        onIsRatingPossibleChanged: (possible) => _.showRating = possible
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
                                if (Mediator.isDisconnected) return Style.colorStatusPanelDisconnected
                                if (Mediator.isConnecting)   return Style.colorStatusPanelConnecting
                                if (Mediator.isConnected)    return Style.colorStatusPanelConnected
                                return 'transparent'
                            }
                        }

                        Text {
                            text: {
                                //: Not connected the a VPN server.
                                if (Mediator.isDisconnected) return qsTr('Unprotected')
                                //: Establishing a connection to a VPN server.
                                if (Mediator.isConnecting)   return qsTr('Connecting')
                                //: Connected to a VPN server.
                                if (Mediator.isConnected)    return qsTr('Protected')
                                return ''
                            }
                            color: {
                                if (Mediator.isDisconnected) return Style.colorStatusPanelDisconnected
                                if (Mediator.isConnecting)   return Style.colorStatusPanelConnecting
                                if (Mediator.isConnected)    return Style.colorStatusPanelConnected
                                return 'transparent'
                            }
                            font.pixelSize: Style.fontSizeSmall
                            font.capitalization: Font.AllUppercase
                        }
                    }

                    RowLayout {
                        Text {
                            text: {
                                const country = Mediator.countryList.find((c) => {
                                                                                 return c.id === Mediator.connectedCountryId
                                                                             })
                                //: Feedback to NordVPN by rating the connection with 1 to 5 starts.
                                if (_.showRating)                    return qsTr('Rate your connection speed')
                                //: Status hint while disconnected.
                                if (Mediator.isDisconnected)         return qsTr('Pick country or use quick connect')
                                //: Status hint while connecting.
                                if (Mediator.isConnecting)           return qsTr('Finding the best server ...')
                                //: Status hint while connected.
                                if (Mediator.isConnected && country) return qsTr('Connected to %1 #%2').arg(country.name).arg(Mediator.connectedServerNr)
                                return ''
                            }
                        }

                        Text {
                            text: Mediator.isConnected && Mediator.connectedIP
                                  ? '(' + Mediator.connectedIP + ')'
                                  : ''
                            font.pixelSize: .9*Qt.application.font.pixelSize
                            color: Style.colorStatusPanelSeconary
                            font.weight: Font.Thin
                        }
                    }

                    Text {
                        visible: _.showRating
                        //: Small additional hint when rating the connection with 1 to 5 stars.
                        text: qsTr('Your feedback helps us improve!')
                        font.pixelSize: Style.fontSizeSmall
                        color: Style.colorStatusPanelSeconary
                    }
                }

                Item {
                    Layout.fillWidth: true
                }

                Button {
                    visible: Mediator.isDisconnected && !_.showRating
                    enabled: !Mediator.areConnectionCommandsPaused
                    //: Button to pick the fastest server and connect to it.
                    text: qsTr('Quick Connect')
                    icon.name: 'network-wired'
                    onClicked: Mediator.quickConnect()
                }

                Button {
                    visible: Mediator.isConnecting
                    //: Button to abort the currently running connection establishment.
                    text: qsTr('Cancel')
                    icon.name: 'process-stop'
                    onClicked: Mediator.cancelConnection()
                }

                Button {
                    visible: Mediator.isConnected
                    enabled: !Mediator.areConnectionCommandsPaused
                    //: Button to disconnect from the currently connected server.
                    text: qsTr('Disconnect')
                    icon.name: 'network-offline'
                    onClicked: Mediator.disconnect()
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
                        color: Style.colorMid
                    }

                    CloseButton {
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
