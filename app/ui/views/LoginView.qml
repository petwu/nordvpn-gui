import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import Style 1.0

import '../general'
import '../icons'

Item {
    function tryLogin() {
        const username = usernameField.text.trim()
        const password = passwordField.text
        if (username === '' || password === '') {
            invalidHint.visible = true
            return
        }
        const success = AccountMediator.login(username, password)
        if (success) {
            NavMediator.showViewAfterSuccessfulLogin()
        } else {
            invalidHint.visible = true
        }
    }

    MenuButton {
    }

    RowLayout {
        anchors.centerIn: parent
        spacing: 64

        Image {
            source: 'qrc:/img/logo.svg'
            sourceSize.height: 160
        }

        ColumnLayout {
            spacing: 16

            Text {
                text: 'Login'
                font.bold: true
                font.pixelSize: Style.fontSizeLarge
            }

            GridLayout {
                columns: 2
                columnSpacing: 12

                Label {
                    text: 'E-mail:'
                }

                TextField {
                    id: usernameField
                    implicitWidth: 240
                    Keys.onReturnPressed: tryLogin()
                    Keys.onEnterPressed: tryLogin()
                }

                Label {
                    text: 'Password:'
                }

                TextField {
                    id: passwordField
                    implicitWidth: 240
                    echoMode: showPassword ? TextField.Normal : TextField.Password
                    Keys.onReturnPressed: tryLogin()
                    Keys.onEnterPressed: tryLogin()

                    property bool showPassword: false

                    Control {
                        anchors.right: parent.right
                        padding: (passwordField.height - showPasswordIcon.height) / 2
                        height: parent.height
                        width: height
                        anchors.verticalCenter: parent.verticalCenter

                        Fa5Icon {
                            id: showPasswordIcon
                            anchors.centerIn: parent
                            iconName: passwordField.showPassword ? 'eye-slash' : 'eye'
                            color: Style.colorTextDisabled
                        }

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            hoverEnabled: true
                            onEntered: showPasswordIcon.color = Style.colorText
                            onExited: showPasswordIcon.color = Style.colorTextDisabled
                            onClicked: passwordField.showPassword = !passwordField.showPassword
                        }
                    }
                }
            }

            Text {
                id: invalidHint
                text: 'Invalid Credentials'
                color: Style.colorError
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
                padding: 8
                visible: false

                Rectangle {
                    anchors.fill: parent
                    color: 'transparent'
                    border.color: Style.colorError
                    border.width: 1
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: invalidHint.visible = false
                }
            }

            RowLayout {
                width: parent.width

                Text {
                    textFormat: Text.RichText
                    text: '<a href="' + NordAccountURL + '">Register / Reset Password</a>'
                    Layout.fillWidth: true
                    onLinkActivated: Qt.openUrlExternally(link)

                    MouseArea {
                        anchors.fill: parent
                        acceptedButtons: Qt.NoButton
                        cursorShape: parent.hoveredLink
                                     ? Qt.PointingHandCursor
                                     : Qt.ArrowCursor
                    }
                }

                Button {
                    text: 'Login'
                    onClicked: tryLogin()
                }
            }
        }
    }
}
