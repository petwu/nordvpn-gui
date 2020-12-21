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
            iconName: 'bug'
            size: 8
            color: Style.colorError
        }

        ColumnLayout {
            spacing: 16
            Layout.maximumWidth: 440

            Text {
                //: Title on the error view that is shown, when an unknown error occured.
                text: qsTr('Something went wrong ...')
                font.bold: true
                font.pixelSize: Style.fontSizeLarge
            }

            Text {
                //: Text 1 on the error view that is shown, when an unknown error occured.
                text: qsTr('The NordVPN CLI application reported the following error:')
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
            }

            TextEdit {
                topPadding: 8
                bottomPadding: 8
                leftPadding: 12
                rightPadding: 12
                textFormat: Text.RichText
                text: NavMediator.mainWindowPayload.miscError
                readOnly: true
                selectByMouse: true
                color: Style.colorText
                wrapMode: TextEdit.Wrap
                Layout.maximumWidth: parent.width
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
                // Text 2 on the error view that is shown, when an unknown error occured.
                //~ Note Keep the HTML tags (words with angle brackets, e.g. <br />).
                text: qsTr('If you do not know how to handle to this issue, ask ' +
                           '<a href="https://duckduckgo.com/?q=">DuckDuckGo</a>, ' +
                           '<a href="https://www.google.com/search?q=">Google</a> ' +
                           'or your favorite search engine for help.')
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                onLinkActivated: Qt.openUrlExternally(link + encodeURI('NordVPN: ' + NavMediator.mainWindowPayload.miscError))

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
                // Text 3 on the error view that is shown, when an unknown error occured.
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
