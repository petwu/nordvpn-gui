import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import Style 1.0

import '../general'

ApplicationWindow {
    id: aboutWindow
    visible: true
    title: 'About — ' + ApplicationName
    width: 520
    height: 240
    minimumWidth: width
    minimumHeight: height
    maximumWidth: width
    maximumHeight: height
    flags: Qt.Dialog
    modality: Qt.ApplicationModal

    QtObject {
        id: _

        readonly property int spacing: 16
    }

    RowLayout {
        x: _.spacing
        y: _.spacing
        width: parent.width - 2*_.spacing
        height: parent.height - 2*_.spacing
        spacing: _.spacing

        Item {
            width: 8
        }

        Image {
            id: logo
            source: 'qrc:/img/logo.svg'
            sourceSize.height: 160
        }

        Item {
            width: 8
        }

        ColumnLayout {
            Layout.fillWidth: true
            spacing: _.spacing / 2

            Text {
                text: ApplicationName
                font.pixelSize: Style.fontSizeLarge
                font.bold: true
            }

            Text {
                text: 'Version: ' + Version
            }

            Text {
                text: ApplicationDescription
                font.italic: true
                wrapMode: Text.Wrap
                Layout.fillWidth: true
            }

            Text {
                text: 'Copyright © ' + Author
            }

            Item {
                height: _.spacing / 2
            }

            RowLayout {
                Layout.fillWidth: true

                Button {
                    text: 'Source Code'
                    onClicked: Qt.openUrlExternally(RepositoryURL)
                }

                Button {
                    text: 'License: ' + License
                    onClicked: {
                        Qt.createComponent('qrc:/ui/windows/LicenseWindow.qml').createObject(aboutWindow).show()
                    }
                }
            }
        }
    }
}
