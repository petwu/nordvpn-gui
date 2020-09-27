import QtQuick 2.12
import QtQuick.Controls 2.12

import Style 1.0

Button {
    id: btn
    horizontalPadding: isClose ? 0 : 3*verticalPadding

    property bool isInverted: false
    property bool isDanger: false
    property bool isClose: false
    property bool hasNoBackground: false
    property color foregroundColor: Style.colorButtonDefaultFrgd
    property color backgroundColor: Style.colorButtonDefaultBkgd

    Component.onCompleted: {
        let frgd = btn.foregroundColor
        let bkgd = btn.backgroundColor
        if (btn.isDanger) {
            frgd = Style.colorButtonDangerFrgd
            bkgd = Style.colorButtonDangerBkgd
        }
        if (btn.isInverted) {
            btn.foregroundColor = bkgd
            btn.backgroundColor = frgd
        } else {
            btn.foregroundColor = frgd
            btn.backgroundColor = bkgd
        }
        if (btn.hasNoBackground) {
            btn.backgroundColor = 'transparent'
        }
    }

    Text {
        visible: btn.isClose
        anchors.centerIn: btn
        text: '×'
        font.pixelSize: 2.25 * Qt.application.font.pixelSize
        color: foregroundColor
        opacity: enabled ? 1 : (btn.isInverted ? .5 : .9)
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    contentItem: Text {
        text: btn.isClose ? '' : btn.text
        font: btn.font
        opacity: enabled ? 1 : (btn.isInverted ? .5 : .9)
        color: foregroundColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: 32
        implicitHeight: 32
        opacity: enabled ? 1 : (btn.isInverted ? .9 : .75)
        color: backgroundColor
        border.width: btn.border ? btn.border.width : 0
        border.color: btn.border ? btn.border.color : 'transparent'
        radius: btn.radius ? btn.radius : 0
    }

    MouseArea {
        anchors.fill: btn
        cursorShape: Qt.PointingHandCursor
        onPressed: {
            btn.down = true
            btn.clicked()
        }
        onReleased: {
            btn.down = false
        }
    }
}
