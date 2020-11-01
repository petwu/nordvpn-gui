import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import Style 1.0

import '../general'
import '../icons'

Item {
    RowLayout {
        anchors.centerIn: parent
        spacing: 40

        Fa5Icon {
            iconName: 'wifi'
            size: 8
            color: Style.colorError
        }

        ColumnLayout {
            spacing: 16
            Layout.maximumWidth: 440

            Text {
                text: 'Disconnected'
                font.bold: true
                font.pixelSize: Style.fontSizeLarge
            }

            Text {
                text: 'An active internet connection is required in order to connect to a VPN server. ' +
                      'Restarting your device and/or your router are typical things to try while troubleshooting connection issues.'
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
            }
        }
    }
}
