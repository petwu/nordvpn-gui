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
        target: ConnectionMediator
        onIsRatingPossibleChanged: (possible) => _.showRating = possible
    }

    /*!
     * @brief Format a byte number (integer) as a smaller number with the appropriate unit.
     * Examples:
     *     1024 --> 1 kiB
     *     1,468,006 --> 1.4 MiB
     *
     * @param bytes The number of bytes as integer.
     * @param decimals The number of decimal places.
     */
    function formatBytes(bytes, decimals = 2) {
        // source (slightly modified):
        // https://stackoverflow.com/questions/15900485/correct-way-to-convert-size-in-bytes-to-kb-mb-gb-in-javascript#answer-18650828
        if (bytes <= 0) return '0 B'
        const base = 1024
        const sizes = ['B', 'kiB', 'MiB', 'GiB', 'TiB', 'PiB', 'EiB', 'ZiB', 'YiB']
        const i = Math.floor(Math.log(bytes) / Math.log(base))
        return (bytes / Math.pow(base, i)).toFixed(Math.max(0, decimals)) + ' ' + sizes[i]
    }

    /*!
     * @brief Format a number as a human readable string. Examples:
     *      16 --> 16s
     *      60 --> 1min
     *      256 --> 4min 16s
     *      3856 --> 1h 4min 16s
     *      ...
     * @param secs Number of seconds as integer (no suffix like 's', 'sec', etc.).
     */
    function formatSeconds(secs) {
        // unitSep not a regular space, but a thin space (unicode: U+2009, LaTeX: \,)
        // for separating the number from its unit
        const unitSep = ' '
        if (secs < 60) {
            return secs + unitSep + qsTranslate('TimeUnit', 's')
        }
        let t = ''
        const tryAppendUnit = (secsPerUnit, unit, minDigits = 1) => {
            // ignore units that are smaller than the number of seconds per unit
            if (t === '' && secs < secsPerUnit) return
            const num = Math.floor(secs / secsPerUnit)
            // pad with leading zeros if required
            let numS = num.toString()
            if (t !== '') {
                while (numS.length < minDigits) numS = '0' + numS
            }
            // regular space to separate multiple num-unit-pairs
            if (t.length > 0) t += ' '
            // append
            t += numS + unitSep + qsTranslate('TimeUnit', unit)
            // decrement secs by the number of seconds covered by the added numb-unit-pair
            secs -= (num * secsPerUnit)
        }
        tryAppendUnit(31536000, 'yr', 1) // years
        tryAppendUnit(86400, 'd', 1) // days
        tryAppendUnit(3600, 'h', 2) // hours
        tryAppendUnit(60, 'min', 2) // minutes
        tryAppendUnit(1, 's', 2) // seconds
        return t
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

        Behavior on height {
            NumberAnimation {
                duration: 100
                easing.type: Easing.InOutQuad
            }
        }

        Behavior on width {
            NumberAnimation {
                duration: 100
                easing.type: Easing.InOutQuad
            }
        }

        RowLayout {
            x: 12
            y: 12

            RowLayout {
                id: content
                Layout.minimumWidth: 460

                Column {
                    Layout.alignment: Qt.AlignBottom
                    spacing: 2

                    RowLayout {
                        visible: !_.showRating
                        spacing: 6

                        Rectangle {
                            width: 6
                            height: width
                            radius: width/2
                            Layout.alignment: Qt.AlignVCenter
                            color: {
                                if (ConnectionMediator.isDisconnected) return Style.colorStatusPanelDisconnected
                                if (ConnectionMediator.isConnecting)   return Style.colorStatusPanelConnecting
                                if (ConnectionMediator.isConnected)    return Style.colorStatusPanelConnected
                                return 'transparent'
                            }
                        }

                        Text {
                            text: {
                                //: Not connected the a VPN server.
                                if (ConnectionMediator.isDisconnected) return qsTr('Unprotected')
                                //: Establishing a connection to a VPN server.
                                if (ConnectionMediator.isConnecting)   return qsTr('Connecting')
                                //: Connected to a VPN server.
                                if (ConnectionMediator.isConnected)    return qsTr('Protected')
                                return ''
                            }
                            color: {
                                if (ConnectionMediator.isDisconnected) return Style.colorStatusPanelDisconnected
                                if (ConnectionMediator.isConnecting)   return Style.colorStatusPanelConnecting
                                if (ConnectionMediator.isConnected)    return Style.colorStatusPanelConnected
                                return 'transparent'
                            }
                            font.pixelSize: Style.fontSizeSmall
                            font.capitalization: Font.AllUppercase
                        }
                    }

                    RowLayout {
                        Text {
                            text: {
                                const country = ConnectionMediator.countryList.find((c) => c.id === ConnectionMediator.connectedCountryId)
                                const city = country ? country.cities.find((c) => c.id === ConnectionMediator.connectedCityId) : ''
                                //: Feedback to NordVPN by rating the connection with 1 to 5 starts.
                                if (_.showRating)                    return qsTr('Rate your connection speed')
                                //: Status hint while disconnected.
                                if (ConnectionMediator.isDisconnected)         return qsTr('Pick country or use quick connect')
                                //: Status hint while connecting.
                                if (ConnectionMediator.isConnecting)           return qsTr('Finding the best server ...')
                                //: Status hint while connected.
                                if (ConnectionMediator.isConnected && country) return qsTr('Connected to %1, %2 #%3').arg(city.name).arg(country.name).arg(ConnectionMediator.connectedServerNr)
                                return 'Error'
                            }
                        }
                    }

                    Text {
                        visible: ConnectionMediator.isConnected
                        text: ConnectionMediator.connectedIP +
                              '    ▲ ' + formatBytes(ConnectionMediator.sentBytes) +
                              '    ▼ ' + formatBytes(ConnectionMediator.receivedBytes) +
                              '    ⏳ ' + formatSeconds(ConnectionMediator.uptimeSeconds)
                        font.pixelSize: .9*Qt.application.font.pixelSize
                        color: Style.colorStatusPanelSeconary
                        font.weight: Font.Thin
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
                    visible: ConnectionMediator.isDisconnected && !_.showRating
                    enabled: !ConnectionMediator.areConnectionCommandsPaused
                    //: Button to pick the fastest server and connect to it.
                    text: qsTr('Quick Connect')
                    icon.name: 'network-wired'
                    onClicked: ConnectionMediator.quickConnect()
                }

                Button {
                    visible: ConnectionMediator.isConnecting
                    //: Button to abort the currently running connection establishment.
                    text: qsTr('Cancel')
                    icon.name: 'process-stop'
                    onClicked: ConnectionMediator.cancelConnection()
                }

                Button {
                    visible: ConnectionMediator.isConnected
                    enabled: !ConnectionMediator.areConnectionCommandsPaused
                    //: Button to disconnect from the currently connected server.
                    text: qsTr('Disconnect')
                    icon.name: 'network-offline'
                    onClicked: ConnectionMediator.disconnect()
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
