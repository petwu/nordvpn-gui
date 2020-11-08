import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.12

import Style 1.0

import '../general'

ApplicationWindow {
    id: preferencesWindow
    visible: true
    //: TItle of the preferences/settings window.
    title: qsTr('Preferences') + ' — ' + ApplicationName
    width: 640
    height: 360
    minimumWidth: width
    minimumHeight: height
    maximumWidth: width
    maximumHeight: height
    flags: Qt.Dialog
    modality: Qt.ApplicationModal
    Component.onCompleted: PreferencesMediator.refreshSettings()

    Shortcut {
        // close on 'Esc' pressed
        sequence: StandardKey.Cancel
        onActivated: preferencesWindow.close()
    }

    QtObject {
        id: _

        property int spacing: 16
        property var settings: PreferencesMediator.nordvpnSettings
    }

    Row {
        id: cols
        width: parent.width - 2*_.spacing
        height: parent.height - 2*_.spacing
        spacing: _.spacing
        padding: _.spacing

        Control {
            id: groups
            width: 196
            height: parent.height

            Rectangle {
                anchors.fill: listView
                color: Style.colorBase
            }

            ListView {
                id: listView
                width: parent.width
                height: parent.height
                focus: true
                currentIndex: 0
                model: [
                    //: Category title for settings regarding the visual appearance or feedback of the application.
                    { text: qsTr('Appearance'), icon: 'preferences-desktop-theme' },
                    //: Category title for settings regarding technology and protocol used to connect VPN servers.
                    { text: qsTr('Technology'), icon: 'document-properties'       },
                    //: Category title for settings regarding additional security settings like a kill-switch.
                    { text: qsTr('Security'),   icon: 'security-high'             },
                    //: Category title for settings to manage a whitelist of allowed ports and subnets.
                    { text: qsTr('Whitelist'),  icon: 'view-sort-ascending'       },
                    //: Category title for setting custom DNS (Domain Name Service) server IPs.
                    { text: qsTr('DNS'),        icon: 'network-server'            },
                ]
                delegate: MenuItem {
                    width: listView.width
                    text: modelData.text
                    icon.name: modelData.icon
                    highlighted: listView.currentIndex === index

                    MouseArea {
                        anchors.fill: parent
                        onClicked: listView.currentIndex = index
                    }
                }
            }
        }

        Column {
            visible: listView.currentIndex === 0
            width: cols.width - groups.width - cols.spacing
            spacing: _.spacing

            DescriptionCheckBox {
                //: Checkbox label.
                label: qsTr('Notifications')
                //: Checkbox description.
                description: qsTr('Show desktop contifications when the connection status changes.')
                checked: _.settings.notify
                onCheckedChanged: PreferencesMediator.setNotify(checked)
            }
        }

        Column {
            visible: listView.currentIndex === 1
            width: cols.width - groups.width - cols.spacing
            spacing: _.spacing

            DescriptionComboBox {
                // Dropdown label.
                label: qsTr('Technology')
                // Dropdown description.
                description: qsTr('OpenVPN is a robust open-source software for VPN services that supports' +
                                  'both, TCP and UDP based connections. It is suggested for security-conscious' +
                                  'users. NordLynx is a technology NordVPN built around WireGuard, a more' +
                                  'recent alternative to OpenVPN. NordLynx promises to be faster than' +
                                  'OpenVPN but it is less mature and only supports UDP connections.')
                model: ['OpenVPN', 'NordLynx']
                defaultText: _.settings.technology
                onActivated: (i) => PreferencesMediator.setTechnology(model[i])
            }

            DescriptionComboBox {
                enabled: _.settings.protocol !== null
                // Dropdown label.
                label: qsTr('Protocol')
                // Dropdown description.
                description: qsTr('TCP (Transmittion Control Protocol) is a connection-oriented protocol that' +
                                  'guarantees that all sent data is received correctly. UDP (User Datagram' +
                                  'Protocol) on the other hand does not provide any error-detection to reduce' +
                                  'the network overhead. UDP is therefore fasert but less reliable than TCP.')
                model: ['UDP', 'TCP']
                defaultText: _.settings.protocol
                onActivated: (i) => PreferencesMediator.setProtocol(model[i])
            }
        }

        Column {
            visible: listView.currentIndex === 2
            width: cols.width - groups.width - cols.spacing
            spacing: _.spacing

            DescriptionCheckBox {
                //: Checkbox label.
                label: qsTr('Auto Connect')
                //: Checkbox description.
                description: qsTr('Automatically try to connect to a VPN server on operating system' +
                                  'startup.')
                checked: _.settings.autoconnect
                onCheckedChanged: PreferencesMediator.setAutoconnect(checked)
            }

            DescriptionCheckBox {
                //: Checkbox label.
                label: qsTr('CyberSec')
                //: Checkbox description.
                description: qsTr('Block suspicious websites, botnets and ads. This disables custom DNS.')
                checked: _.settings.cybersec
                onCheckedChanged: PreferencesMediator.setCybersec(checked)
            }

            DescriptionCheckBox {
                //: Checkbox label.
                label: qsTr('Kill Switch')
                //: Checkbox description.
                description: qsTr('Prevent your device from accessing the internet while not connected to' +
                                  'a VPN server or in case connection with a VPN server is lost.')
                checked: _.settings.killswitch
                onCheckedChanged: PreferencesMediator.setKillswitch(checked)
            }

            DescriptionCheckBox {
                //: Checkbox label.
                id: obfuscatedCheckbox
                label: qsTr('Obfuscated')
                //: Checkbox description.
                description: qsTr('Bypass network traffic sensors which aim to detect usage of VPN' +
                                  'services and log, throttle or block them. (OpenVPN only)')
                enabled: _.settings.obfuscated !== null
                checked: _.settings.obfuscated === true
                onCheckedChanged: PreferencesMediator.setObfuscated(checked)
            }
        }

        Item {
            visible: listView.currentIndex === 3
            width: cols.width - groups.width - cols.spacing
            height: parent.height

            TabBar {
                id: whitelistTabs
                width: parent.width
                z: 10

                TabButton {
                    //: Tab label. IP addresses of subnets.
                    text: qsTr('Subnets')
                }
                TabButton {
                    //: Tab label. Network ports (numbers from 0 to 65535).
                    text: qsTr('Ports')
                }
            }

            Frame {
                anchors {
                    topMargin: -1
                    top: whitelistTabs.bottom
                    left: parent.left
                    right: parent.right
                    bottom: parent.bottom
                }
                padding: background.border ? background.border.width : 1

                StackLayout {
                    currentIndex: whitelistTabs.currentIndex
                    anchors.fill: parent

                    ScrollView {
                        width: parent.width
                        height: parent.height
                        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                        ScrollBar.vertical.policy: ScrollBar.AsNeeded
                        onAvailableWidthChanged: {
                            whitelistSubnetsColumn.width = availableWidth
                        }

                        Column {
                            id: whitelistSubnetsColumn
                            padding: _.spacing
                            spacing: 4

                            Repeater {
                                id: whitelistSubnetsRepeater
                                model: _.settings.whitelistSubnets

                                RowLayout {
                                    spacing: 6
                                    width: parent.width - 2*_.spacing

                                    TextField {
                                        Layout.fillWidth: true
                                        text: modelData
                                        color: valid ? Style.colorText : Style.colorError
                                        onTextChanged: {
                                            PreferencesMediator.updateWhitelistSubnet(index, text)
                                            valid = PreferencesMediator.isValidSubnetMask(text)
                                        }
                                        Component.onCompleted: {
                                            valid = PreferencesMediator.isValidSubnetMask(text)
                                        }
                                        property bool valid: false
                                    }

                                    Button {
                                        icon.name: 'list-remove'
                                        onClicked: PreferencesMediator.removeSubnetFromWhitelist(index)
                                    }
                                }
                            }

                            Button {
                                icon.name: 'list-add'
                                anchors.right: parent.right
                                anchors.rightMargin: _.spacing
                                onClicked: PreferencesMediator.addSubnetToWhitelist()
                            }
                        }
                    }

                    ScrollView {
                        width: parent.width
                        height: parent.height
                        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                        ScrollBar.vertical.policy: ScrollBar.AsNeeded
                        onAvailableWidthChanged: {
                            whitelistPortsColumn.width = availableWidth
                        }

                        Column {
                            id: whitelistPortsColumn
                            padding: _.spacing
                            spacing: 4

                            Repeater {
                                id: whitelistPortsRepeater
                                model: _.settings.whitelistPorts

                                RowLayout {
                                    spacing: 6
                                    width: parent.width - 2*_.spacing

                                    TextField {
                                        id: portFromField
                                        Layout.fillWidth: true
                                        text: modelData.portFrom
                                        //: Placeholder text for the start of a number range.
                                        placeholderText: qsTr('from')
                                        color: valid ? Style.colorText : Style.colorError
                                        validator: IntValidator { }
                                        onTextChanged: {
                                            PreferencesMediator.updateWhitelistPorts(index,
                                                                                     portFromField.text,
                                                                                     portToField.text,
                                                                                     protocolRadios.getCheckedProtocolFlag())
                                            valid = PreferencesMediator.isValidPort(text)
                                            portToField.valid = PreferencesMediator.isValidPort(portToField.text) &&
                                                    parseInt(text) <= parseInt(portToField.text)
                                        }
                                        Component.onCompleted: {
                                            valid = PreferencesMediator.isValidPort(text)
                                        }
                                        property bool valid: false
                                    }

                                    Text {
                                        text: /* no qsTr() */ '—'
                                    }

                                    TextField {
                                        id: portToField
                                        Layout.fillWidth: true
                                        text: modelData.portTo
                                        //: Placeholder text for the end of a number range.
                                        placeholderText: qsTr('to')
                                        color: valid ? Style.colorText : Style.colorError
                                        validator: IntValidator { }
                                        onTextChanged: {
                                            PreferencesMediator.updateWhitelistPorts(index,
                                                                                     portFromField.text,
                                                                                     portToField.text,
                                                                                     protocolRadios.getCheckedProtocolFlag())
                                            valid = PreferencesMediator.isValidPort(text) &&
                                                    parseInt(text) >= parseInt(portFromField.text)
                                        }
                                        Component.onCompleted: {
                                            valid = PreferencesMediator.isValidPort(text)
                                        }
                                        property bool valid: false
                                    }

                                    ButtonGroup {
                                        id: protocolRadios
                                        onCheckedButtonChanged: PreferencesMediator.updateWhitelistPorts(index,
                                                                                                         portFromField.text,
                                                                                                         portToField.text,
                                                                                                         protocolRadios.getCheckedProtocolFlag())
                                        function getCheckedProtocolFlag() {
                                            switch (checkedButton.text) {
                                            case 'UDP':  return 1
                                            case 'TCP':  return 2
                                            default:     return 3
                                            }
                                        }
                                    }

                                    RadioButton {
                                        text: /* no qsTr() */ 'UDP'
                                        checked: modelData.protocolFlag === 1
                                        ButtonGroup.group: protocolRadios
                                    }

                                    RadioButton {
                                        text: /* no qsTr() */ 'TCP'
                                        checked: modelData.protocolFlag === 2
                                        ButtonGroup.group: protocolRadios
                                    }

                                    RadioButton {
                                        //: Radio button to whitelist both, TCP and UDP ports.
                                        text: qsTr('both')
                                        checked: modelData.protocolFlag === 3
                                        ButtonGroup.group: protocolRadios
                                    }

                                    Button {
                                        icon.name: 'list-remove'
                                        onClicked: PreferencesMediator.removePortsFromWhitelist(index)
                                    }
                                }
                            }

                            Button {
                                icon.name: 'list-add'
                                anchors.right: parent.right
                                anchors.rightMargin: _.spacing
                                onClicked: PreferencesMediator.addPortsToWhitelist()
                            }
                        }
                    }
                }
            }
        }


        Column {
            visible: listView.currentIndex === 4
            width: cols.width - groups.width - cols.spacing
            spacing: _.spacing

            DescriptionCheckBox {
                id: dnsCheckbox
                //: Checkbox label.
                label: qsTr('Custom DNS')
                //: Checkbox description.
                description: qsTr('If enabled, you may specify up to 3 custom DNS (Domain Name' +
                                  'System) servers. Supported values are valid IPv4 or IPv6 addresses.\n' +
                                  'This disables CyberSec.\n' +
                                  'Note: Invalid addresses will be ignored upon saving.')
                checked: _.settings.dns
                onCheckedChanged: PreferencesMediator.setDns(checked)
            }

            Repeater {
                model: _.settings.maxNumberOfDnsAddresses

                RowLayout {
                    spacing: _.spacing
                    x: dnsCheckbox.descriptionIndentation
                    width: 320
                    enabled: dnsCheckbox.checked

                    Label {
                        text: (index+1) + '.'
                    }

                    TextField {
                        id: ipInput
                        color: enabled
                               ? (valid ? Style.colorText : Style.colorError)
                               : Style.colorTextDisabled
                        text: _.settings.dnsAddresses[index]
                        Layout.fillWidth: true
                        onTextChanged: {
                            valid = PreferencesMediator.isValidIpAddress(text)
                            PreferencesMediator.setDnsAddress(index, text)
                        }
                        Component.onCompleted: {
                            valid = PreferencesMediator.isValidIpAddress(text)
                        }
                        property bool valid: false
                    }

                    Label {
                        opacity: ipInput.text.length && enabled > 0 ? 1 : 0
                        text: ipInput.valid ? '✓' : '🗙'
                        color: ipInput.valid ? Style.colorSuccess : Style.colorError
                        ToolTip.visible: validHintHoverHandler.hovered
                        ToolTip.text: ipInput.valid ? 'IP address is valid' : 'IP address is invalid'
                        HoverHandler {
                            id: validHintHoverHandler
                        }
                    }
                }
            }
        }
    }

    footer: Control {
        id: footer
        x: _.spacing
        width: parent.width - 2*_.spacing
        height: childrenRect.height + _.spacing

        RowLayout {
            width: parent.width
            height: childrenRect.height
            anchors.top: parent.top

            Button {
                //: Text of the button to restore the default settings/preferences.
                text: qsTr('Defaults')
                DialogButtonBox.buttonRole: DialogButtonBox.RestoreDefaults
                onClicked: confirmDefaultsDialog.open()

                MessageDialog {
                    id: confirmDefaultsDialog
                    //: Title of a dialog window to confirm the restoring of the default settings.
                    title: qsTr('Restore Default Settings') + ' — ' + ApplicationName
                    //: Description/warning text of a dialog window to confirm the restoring of the default settings.
                    text: qsTr('Do you really want to discard your current settings and restore the default ones?\n\n' +
                               'CAUTION: You will be logged out automatically and have to log in again!')
                    icon: StandardIcon.Warning
                    standardButtons: StandardButton.Yes | StandardButton.No
                    onNo: close()
                    onYes: {
                        PreferencesMediator.restoreDefaultNordvpnSettings()
                        close()
                    }
                }
            }

            Item {
                Layout.fillWidth: true
            }

            DialogButtonBox {
                padding: 0

                Button {
                    //: Dialog button to approve the made changes and close the settings.
                    text: qsTr('Ok')
                    icon.name: 'dialog-ok-apply'
                    DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole
                }
                onAccepted: {
                    PreferencesMediator.saveNordvpnSettings()
                    preferencesWindow.close()
                }

                Button {
                    //: Dialog button to discard the made changes and close the settings.
                    text: qsTr('Cancel')
                    icon.name: 'dialog-cancel'
                    DialogButtonBox.buttonRole: DialogButtonBox.RejectRole
                }
                onRejected: preferencesWindow.close()
            }
        }
    }
}
