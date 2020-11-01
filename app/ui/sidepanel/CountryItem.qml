import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.12

import Style 1.0

import '../general'
import '../icons'

Item {
    id: countryItem

    property var country: undefined
    property int iconTextSpacing: 8
    property int iconSize: 20
    property double iconHorizontalCenter: 0

    signal popupOpened
    signal popupClosed

    QtObject {
        id: _
        property var cityList: [{ name: 'Fastest', value: -1 }].concat(country.cities)

        property var serverList: []

        function updateServerList(cityId = -1) {
            let servers = Mediator.getServers(country.id, cityId)
            servers.forEach((s) => {
                                s.text = s.load + '%  |  ' + s.name
                            })
            servers.sort((a, b) => {
                             if (a.load < b.load) return -1
                             if (a.load > b.load) return 1
                             return 0
                         })
            _.serverList = [{ text: 'Fastest', id: -1 }].concat(servers)
        }
    }

    Rectangle {
        anchors.fill: parent
        visible: hoverCountry.hovered || cityServerSelectionPopup.visible
        color: Style.colorBase
    }

    Rectangle {
        id: flag
        x: countryItem.iconHorizontalCenter-width/2
        width: countryItem.iconSize
        height: width
        anchors.verticalCenter: parent.verticalCenter
        color: 'transparent'

        Image {
            id: flagImg
            source: 'qrc:/flag/'+countryItem.country.countryCode
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
            opacity: Mediator.areConnectionCommandsPaused
                     ? .33
                     : 0
        }
    }

    Text {
        id: text
        text: countryItem.country.name
        anchors.left: flag.right
        anchors.leftMargin: countryItem.iconTextSpacing
        anchors.verticalCenter: parent.verticalCenter
    }

    Rectangle {
        width: 6
        height: width
        radius: width/2
        anchors.left: text.right
        anchors.leftMargin: countryItem.iconTextSpacing
        anchors.verticalCenter: parent.verticalCenter
        color: countryItem.country.id === Mediator.connectedCountryId
               ? Style.colorGreen
               : (countryItem.country.id === Mediator.connectingCountryId
                  ? Style.colorOrange
                  : 'transparent')
    }

    HoverHandler {
        id: hoverCountry
    }

    MouseArea {
        anchors.fill: parent
        cursorShape: Mediator.areConnectionCommandsPaused
                     ? Qt.ArrowCursor
                     : Qt.PointingHandCursor
        onClicked: Mediator.connectToCountryById(countryItem.country.id)
    }

    ToolButton {
        text: '⋯'
        display: AbstractButton.TextOnly
        width: height
        anchors.right: parent.right
        anchors.margins: (parent.height - height) / 2
        anchors.verticalCenter: parent.verticalCenter
        visible: hoverCountry.hovered || cityServerSelectionPopup.visible
        onClicked: cityServerSelectionPopup.open()
    }

    Popup {
        id: cityServerSelectionPopup
        x: parent.width + popupArrow.width - 8
        y: (parent.height - height) / 2
        width: Style.sidebarPopupWidth
        height: popupContent.height + topPadding + bottomPadding
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        topMargin: 0
        bottomMargin: 0
        onOpened: {
            popupOpened()
            _.updateServerList()
        }
        onClosed: popupClosed()

        ColumnLayout {
            id: popupContent
            width: parent.width

            RowLayout {
                Layout.fillWidth: true
                spacing: cityServerSelectionPopup.leftPadding

                Image {
                    source: 'qrc:/flag/'+countryItem.country.countryCode
                    sourceSize.height: countryItem.iconSize
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                }

                Column {
                    Text {
                        text: country.name
                    }

                    Text {
                        text: country.cities.length + ' cities/regions'
                        font.pixelSize: Style.fontSizeSmall
                        color: Style.colorStatusPanelSeconary
                    }
                }

                Item {
                    Layout.fillWidth: true
                }

                CloseButton {
                    onClicked: cityServerSelectionPopup.close()
                }
            }

            Item {
                height: 8
            }

            GridLayout {
                columns: 2

                Text {
                    visible: country.cities.length > 1
                    text: 'City:'
                }

                ComboBox {
                    id: citySelector
                    visible: country.cities.length > 1
                    Layout.fillWidth: true
                    model: _.cityList
                    textRole: 'name'
                    onActivated: (i) => {
                                     if (i === 0)
                                        _.updateServerList()
                                     else
                                        _.updateServerList(country.cities[i-1].id)
                                 }
                }

                Text {
                    text: 'Server:'
                }

                ComboBox {
                    id: serverSelector
                    Layout.fillWidth: true
                    model: _.serverList
                    textRole: 'text'
                }
            }

            Item {
                height: 8
            }

            Button {
                Layout.fillWidth: true
                text: 'Connect'
                onClicked: {
                    const server = serverSelector.model[serverSelector.currentIndex]
                    if (server.id < 0) {
                        Mediator.connectToCountryById(country.id)
                    } else {
                        Mediator.connectToServerById(server.id)
                    }
                    cityServerSelectionPopup.close()
                }
            }
        }
    }

    Canvas {
        id: popupArrow
        x: parent.width - 8
        y: {
            const optimal = (parent.height - height) / 2
            if (cityServerSelectionPopup.y > optimal) {
                return cityServerSelectionPopup.y
            } else if (cityServerSelectionPopup.y < -(cityServerSelectionPopup.height-parent.height)) {
                return optimal + cityServerSelectionPopup.y + (cityServerSelectionPopup.height-parent.height)
            } else {
                return optimal
            }
        }
        width: 16
        height: 2*width
        z: 10001
        visible: cityServerSelectionPopup.visible
        opacity: cityServerSelectionPopup.opacity
        onPaint: {
            const ctx = getContext('2d')
            const w = width
            const h = height
            ctx.moveTo(0, h/2)
            ctx.lineTo(w, 0)
            ctx.lineTo(w, h)
            ctx.closePath()
            ctx.fillStyle = cityServerSelectionPopup.background.color
            ctx.fill()
            ctx.moveTo(w, 0)
            ctx.lineTo(0, h/2)
            ctx.lineTo(w, h)
            ctx.lineWidth = cityServerSelectionPopup.background.border.width
            ctx.strokeStyle = cityServerSelectionPopup.background.border.color
            ctx.stroke()
        }
    }
}
