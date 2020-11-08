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
            iconName: 'terminal'
            size: 8
            color: Style.colorError
            Layout.alignment: Qt.AlignTop
        }

        ColumnLayout {
            spacing: 16
            Layout.maximumWidth: 440

            Text {
                //: Title on the error view that is shown, when the device does not provide a shell to execute commands.
                text: qsTr('Error')
                font.bold: true
                font.pixelSize: Style.fontSizeLarge
            }

            Text {
                textFormat: Text.RichText
                // Text 1 on the error view that is shown, when the device does not provide a shell to execute commands.
                //~ Note Keep the HTML tags (words with angle brackets, e.g. <br />).
                text: qsTr('Unable to execute shell commands. ' +
                           'This is required in order to make calls to the NordVPN CLI application.<br />' +
                           'Is <code>/bin/sh</code> available on your machine?' +
                           'Test it by executing the following command inside your terminal:')
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
            }

            TextEdit {
                topPadding: 8
                bottomPadding: 8
                leftPadding: 12
                rightPadding: 12
                textFormat: Text.RichText
                text: /* no qsTr() */ '<code>/bin/sh -c "echo success"</code>'
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
                // Text 2 on the error view that is shown, when the device does not provide a shell to execute commands.
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
