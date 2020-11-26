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
                //: Title on the error view that is shown, when the nordvpn CLI client is not installed.
                //~ Note Only translate the 'not found' part.
                text: qsTr('<i>nordvpn</i> not found')
                font.bold: true
                font.pixelSize: Style.fontSizeLarge
            }

            Text {
                textFormat: Text.RichText
                // Text 1 on the error view that is shown, when the nordvpn CLI client is not installed.
                //~ Note Keep the HTML tags (words with angle brackets, e.g. <br />).
                text: qsTr('This application provides a graphical user interface (GUI) for <i>nordvpn</i>, ' +
                           'the command line interface (CLI) that NordVPN provides for devices running on Linux. ' +
                           'Hence, in order to work, the <i>nordvpn</i> application must be installed and accessible thourgh the ' +
                           '$PATH environment variable.')
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
            }

            Text {
                textFormat: Text.RichText
                // Text 2 on the error view that is shown, when the nordvpn CLI client is not installed.
                //~ Note Keep the HTML tags (words with angle brackets, e.g. <br />).
                text: qsTr('See <a href="%1">here</a> for more information on how to install <i>nordvpn</i> for Linux.').arg(DownloadForLinuxURL)
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
                // Text 3 on the error view that is shown, when the nordvpn CLI client is not installed.
                //~ Note Keep the HTML tags (words with angle brackets, e.g. <br />).
                text: qsTr('You can check if <i>nordvpn</i> is accessible by opening a terminal and executing the following command:')
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
            }

            TextEdit {
                topPadding: 8
                bottomPadding: 8
                leftPadding: 12
                rightPadding: 12
                textFormat: Text.RichText
                text: /* no qsTr() */ '<code>/bin/sh -c "nordvpn --version"</code>'
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
                // Text 4 on the error view that is shown, when the nordvpn CLI client is not installed.
                //~ Note Keep the HTML tags (words with angle brackets, e.g. <br />).
                text: qsTr('Consider creating an <a href="%1">issue</a> if you cannot resolve the problem yourself.').arg(IssuesURL)
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
