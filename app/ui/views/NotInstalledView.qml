import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import Style 1.0

import '../general'
import '../icons'

Item {
    MouseArea {
        anchors.fill: parent
        onClicked: parent.forceActiveFocus()
    }

    RowLayout {
        anchors.centerIn: parent
        spacing: 40

        Fa5Icon {
            iconName: 'search'
            size: 8
            color: Style.colorError
            Layout.alignment: Qt.AlignTop
        }

        ColumnLayout {
            spacing: 16
            Layout.maximumWidth: 440

            Text {
                textFormat: Text.RichText
                text: '<i>nordvpn</i> not found'
                font.bold: true
                font.pixelSize: Style.fontSizeLarge
            }

            Text {
                textFormat: Text.RichText
                text: 'This application provides a graphical user interface (GUI) for <i>nordvpn</i>, ' +
                      'the command line interface (CLI) that NordVPN provides for devices running on Linux. ' +
                      'Hence, in order to work, the <i>nordvpn</i> application must be installed and accessible thourgh the ' +
                      '$PATH environment variable.'
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
            }

            Text {
                textFormat: Text.RichText
                text: 'See <a href="' + DownloadForLinuxURL + '">here</a> for more information on how to install <i>nordvpn</i> for Linux.'
                wrapMode: Text.WordWrap
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

            Text {
                textFormat: Text.RichText
                text: 'You can check if <i>nordvpn</i> is accessible by opening a terminal and executing the following command:'
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
            }

            TextEdit {
                topPadding: 8
                bottomPadding: 8
                leftPadding: 12
                rightPadding: 12
                textFormat: Text.RichText
                text: '<code>/bin/sh -c "nordvpn --version"</code>'
                readOnly: true
                selectByMouse: true
                color: Style.colorText
                onActiveFocusChanged: {
                    if (activeFocus) {
                        selectAll()
                    } else {
                        deselect()
                    }
                }

                Rectangle {
                    anchors.fill: parent
                    color: Style.colorBase
                    z: -1
                }
            }

            Text {
                textFormat: Text.RichText
                text: 'Consider creating an <a href="' + IssuesURL + '">issue</a> if you cannot resolve the problem yourself.'
                wrapMode: Text.WordWrap
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
        }
    }
}
