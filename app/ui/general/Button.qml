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

    QtObject {
        id: _
        property color foreground: 'transparent'
        property color background: 'transparent'
    }

    Component.onCompleted: {
        let frgd, bkgd
        if (btn.isDanger) {
            frgd = Style.colorButtonDangerFrgd
            bkgd = Style.colorButtonDangerBkgd
        } else {
            frgd = Style.colorButtonDefaultFrgd
            bkgd = Style.colorButtonDefaultBkgd
        }
        if (btn.isInverted) {
            _.foreground = bkgd
            _.background = frgd
        } else {
            _.foreground = frgd
            _.background = bkgd
        }
        if (btn.hasNoBackground) {
            _.background = 'transparent'
        }
    }

    Text {
        visible: btn.isClose
        anchors.centerIn: btn
        text: '×'
        font.pixelSize: (isClose ? 2.25 : 1) * Qt.application.font.pixelSize
        color: _.foreground
        opacity: enabled ? 1 : (btn.isInverted ? .5 : .9)
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    contentItem: Text {
        text: btn.isClose ? '' : btn.text
        font: btn.font
        opacity: enabled ? 1 : (btn.isInverted ? .5 : .9)
        color: _.foreground
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: 32
        implicitHeight: 32
        opacity: enabled ? 1 : (btn.isInverted ? .9 : .75)
        color: _.background
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
