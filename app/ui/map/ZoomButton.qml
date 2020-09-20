import QtQuick 2.12
import QtQuick.Controls 2.12

import Style 1.0

Button {
    anchors.topMargin: 4
    anchors.leftMargin: 4
    anchors.rightMargin: 4
    anchors.bottomMargin: 4
    width: 32
    height: width
    font.bold: true
    padding: 2
    font.pixelSize: 1.5*Qt.application.font.pixelSize

    contentItem: Text {
        text: parent.text
        font: parent.font
        opacity: enabled ? 1.0 : 0.3
        color: Style.colorMapWater
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        opacity: enabled ? 1 : 0.7
        color: parent.down ? '#f4f4f4' : '#ffffff'
        border.width: parent.border ? parent.border.width : 0
        border.color: parent.border ? parent.border.color : 'transparent'
        radius: parent.radius ? parent.radius : 0
    }

    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        onPressed: {
            parent.down = true
            parent.clicked()
        }
        onReleased: {
            parent.down = false
        }
    }
}
