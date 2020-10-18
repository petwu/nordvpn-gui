import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import Style 1.0

import '../icons'
import '../general'

Item {
    id: specialityServerItem

    property string name: ''
    property int groupId: -1
    property int iconTextSpacing: 8
    property int iconSize: 20
    property double iconHorizontalCenter: 0

    signal popupOpened
    signal popupClosed

    QtObject {
        id: _
        property var countryList: []
        property var serverList: []

        function updateCountryAndServerList(countryId = -1) {
            const countries = Mediator.getSpecialtyCountries(groupId)
            _.countryList = [{ name : 'Fastest', id: -1 }].concat(countries)
            const servers = Mediator.getSpecialtyServers(groupId, countryId)
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
        visible: hoverGroup.hovered || specialtyServerSelectionPopup.visible
        color: Style.colorBase
    }

    Item {
        id: icon
        x: specialityServerItem.iconHorizontalCenter-width/2
        width: specialityServerItem.iconSize
        height: width
        anchors.verticalCenter: parent.verticalCenter

        IconDoubleVPN {
            visible: specialityServerItem.groupId === 2
        }

        IconOnion {
            visible: specialityServerItem.groupId === 3
        }

        IconP2P {
            visible: specialityServerItem.groupId === 4
        }
    }

    Text {
        id: text
        text: specialityServerItem.name
        anchors.left: icon.right
        anchors.leftMargin: specialityServerItem.iconTextSpacing
        anchors.verticalCenter: parent.verticalCenter
    }

    Rectangle {
        width: 6
        height: width
        radius: width/2
        anchors.left: text.right
        anchors.leftMargin: specialityServerItem.iconTextSpacing
        anchors.verticalCenter: parent.verticalCenter
        color: Mediator.connectedServerGroups.includes(specialityServerItem.groupId)
               ? Style.colorGreen
               : (Mediator.connectingServerGroups.includes(specialityServerItem.groupId)
                  ? Style.colorOrange
                  : 'transparent')
    }

    HoverHandler {
        id: hoverGroup
    }

    MouseArea {
        anchors.fill: parent
        cursorShape: Mediator.areConnectionCommandsPaused
                     ? Qt.ArrowCursor
                     : Qt.PointingHandCursor
        onClicked: Mediator.connectToSpecialtyGroup(specialityServerItem.groupId)
    }

    ToolButton {
        text: '⋯'
        display: AbstractButton.TextOnly
        width: height
        anchors.right: parent.right
        anchors.margins: (parent.height - height) / 2
        anchors.verticalCenter: parent.verticalCenter
        visible: hoverGroup.hovered || specialtyServerSelectionPopup.visible
        onClicked: specialtyServerSelectionPopup.open()
    }

    Popup {
        id: specialtyServerSelectionPopup
        x: parent.width + popupArrow.width - 8
        y: (parent.height - height) / 2
        width: Style.sidebarPopupWidth
        height: popupContent.height + topPadding + bottomPadding
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        topMargin: 0
        bottomMargin: 0
        onOpened: {
            popupOpened()
            _.updateCountryAndServerList()
        }
        onClosed: popupClosed()

        ColumnLayout {
            id: popupContent
            width: parent.width

            RowLayout {
                Layout.fillWidth: true
                spacing: specialtyServerSelectionPopup.leftPadding

                IconDoubleVPN {
                    visible: specialityServerItem.groupId === 2
                }

                IconOnion {
                    visible: specialityServerItem.groupId === 3
                }

                IconP2P {
                    visible: specialityServerItem.groupId === 4
                }

                Text {
                    text: specialityServerItem.name
                }

                Item {
                    Layout.fillWidth: true
                }

                CloseButton {
                    onClicked: specialtyServerSelectionPopup.close()
                }
            }

            Item {
                height: 8
            }

            GridLayout {
                columns: 2

                Text {
                    visible: _.countryList.length > 1
                    text: 'Country:'
                }

                ComboBox {
                    id: countrySelector
                    visible: _.countryList.length > 1
                    Layout.fillWidth: true
                    model: _.countryList
                    textRole: 'name'
                    onActivated: (i) => {
                                     if (i === 0)
                                        _.updateCountryAndServerList()
                                     else
                                        _.updateCountryAndServerList(_.countryList[i].id)
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
                        Mediator.connectToSpecialtyGroup(groupId)
                    } else {
                        Mediator.connectToServerById(server.id)
                    }
                    specialtyServerSelectionPopup.close()
                }
            }
        }
    }

    Canvas {
        id: popupArrow
        x: parent.width - 8
        y: {
            const optimal = (parent.height - height) / 2
            if (specialtyServerSelectionPopup.y > optimal) {
                return specialtyServerSelectionPopup.y
            } else if (specialtyServerSelectionPopup.y < -(specialtyServerSelectionPopup.height-parent.height)) {
                return optimal + specialtyServerSelectionPopup.y + (specialtyServerSelectionPopup.height-parent.height)
            } else {
                return optimal
            }
        }
        width: 16
        height: 2*width
        z: 10001
        visible: specialtyServerSelectionPopup.visible
        opacity: specialtyServerSelectionPopup.opacity
        onPaint: {
            const ctx = getContext('2d')
            const w = width
            const h = height
            ctx.moveTo(0, h/2)
            ctx.lineTo(w, 0)
            ctx.lineTo(w, h)
            ctx.closePath()
            ctx.fillStyle = specialtyServerSelectionPopup.background.color
            ctx.fill()
            ctx.moveTo(w, 0)
            ctx.lineTo(0, h/2)
            ctx.lineTo(w, h)
            ctx.lineWidth = specialtyServerSelectionPopup.background.border.width
            ctx.strokeStyle = specialtyServerSelectionPopup.background.border.color
            ctx.stroke()
        }
    }
}
