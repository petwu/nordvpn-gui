import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import Style 1.0

import '../general'

import 'qrc:/credits/credits.mjs' as Credits

ApplicationWindow {
    id: creditsWindow
    visible: true
    title: 'Credits — ' + ApplicationName
    width: 960
    height: 540
    minimumWidth: 640
    minimumHeight: 360
    flags: Qt.Dialog
    modality: Qt.ApplicationModal

    Shortcut {
        // close on 'Esc' pressed
        sequence: StandardKey.Cancel
        onActivated: creditsWindow.close()
    }

    QtObject {
        id: _
        readonly property int spacing: 16
        property var credits: Credits.default.sort((a, b) => {
                                                       if (a.name.toLowerCase().startsWith('qt'))
                                                           return -1
                                                       if (b.name.toLowerCase().startsWith('qt'))
                                                           return 1
                                                       return a.name.localeCompare(b.name)
                                                   })
    }

    ColumnLayout {
        x: _.spacing
        y: _.spacing
        width: parent.width - 2*_.spacing
        height: parent.height - 2*_.spacing
        spacing: _.spacing

        Text {
            text: 'Credits'
            wrapMode: Text.Wrap
            Layout.fillWidth: true
            font.pixelSize: Style.fontSizeLarge
            font.bold: true
        }

        Text {
            textFormat: Text.RichText
            wrapMode: Text.Wrap
            Layout.fillWidth: true
            text: ApplicationName + ' is made possible by Qt and other Open Source Software (OSS)' +
                  'provided by third parties free of charge. These include software libraries' +
                  'providing some funtionality or resource files like fonts, icons or images. ' +
                  'They are used under the licenses listed below.<br />' +
                  '<b><i>Thanks to everyone who contributed to them!</i></b>'
        }

        MenuSeparator {
            Layout.fillWidth: true
        }

        Row {
            id: cols
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: _.spacing

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
                    model: _.credits
                    clip: true
                    delegate: MenuItem {
                        width: listView.width
                        text: modelData.name
                        highlighted: listView.currentIndex === index

                        MouseArea {
                            anchors.fill: parent
                            onClicked: listView.currentIndex = index
                        }
                    }
                }
            }

            Repeater {
                model: _.credits

                ColumnLayout {
                    visible: listView.currentIndex === index
                    width: cols.width - groups.width - cols.spacing
                    height: parent.height
                    spacing: _.spacing / 2

                    Row {
                        spacing: _.spacing * 2/3

                        Text {
                            text: modelData.name
                            font.pixelSize: Style.fontSizeLarge
                            font.bold: true
                            color: Style.colorHighlight
                        }

                        Text {
                            text: (modelData.version.startsWith('v') ? '' : 'v') + modelData.version
                            anchors.bottom: parent.bottom
                        }
                    }

                    Text {
                        text: 'by ' + modelData.author
                        font.italic: true
                    }

                    Text {
                        textFormat: Text.RichText
                        text: 'Website: &nbsp; <a href="' + modelData.website + '">' + modelData.website + '</a>'
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
                        text: 'License:    ' + modelData.license
                    }

                    ScrollView {
                        id: licenseScrollView
                        ScrollBar.horizontal.policy: ScrollBar.AsNeeded
                        ScrollBar.vertical.policy: ScrollBar.AsNeeded
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        background: Rectangle {
                            color: Style.colorBase
                        }

                        Text {
                            text: modelData.licenseFullText
                            font.family: 'monospace'
                            padding: _.spacing
                        }
                    }
                }
            }
        }
    }
}
