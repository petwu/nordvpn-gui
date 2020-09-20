import QtQuick 2.12

import Style 1.0

Item {
    id: marker
    width: 0
    height: 0

    property double scaleFactor: 1
    property double pointerHeight: .25
    property int radius: 18 * scaleFactor
    property int borderWidth: radius/8

    MouseArea {
        width: 2*marker.radius
        height: width * (1 + marker.pointerHeight)
        x: -width/2
        y: -height
        cursorShape: Qt.PointingHandCursor
        hoverEnabled: true
        onEntered: scaleFactor += .3
        onExited: scaleFactor -= .3

        Behavior on width {
            NumberAnimation {
                duration: 100
                easing.type: Easing.InOutQuad
            }
        }

        /*
          outer circle
        */
        Rectangle {
            id: circle
            width: parent.width
            height: width
            radius: .5 * width
            color: Style.colorMarkerBlueDark
            border.width: marker.borderWidth
            border.color: Style.colorMarkerWhite
        }

        /*
          triangle below outer circle giving the marker its typical shape
        */
        Canvas {
            width: circle.width
            height: circle.height * (1 + marker.pointerHeight)
            anchors.top: circle.top
            anchors.left: circle.left
            onPaint: {
                const ctx = getContext("2d")

                // radius
                const r = circle.width/2
                // diameter
                const d = 2*r
                // distance from center of circle to bottom tip of triangle
                const a = (1 + 2*marker.pointerHeight) * r
                // a normalized
                const an = a/r

                // angle alpha
                const alpha = Math.PI/2 - Math.acos(1/an)
                // points P1
                let p1x = r * (1 - Math.cos(alpha))
                let p1y = r * (1 + Math.sin(alpha)) - marker.borderWidth/2 / Math.sin(alpha)
                // points P2
                let p2x = r
                let p2y = r + a - marker.borderWidth * Math.cos(alpha)
                // points P3
                let p3x = d - p1x
                let p3y = p1y

                // draw the triangle
                ctx.beginPath()
                ctx.moveTo(p1x, p1y)
                ctx.lineTo(p2x, p2y)
                ctx.lineTo(p3x, p3y)
                ctx.fillStyle = circle.color
                ctx.fill()

                // outline
                ctx.beginPath()
                ctx.moveTo(p1x, p1y)
                ctx.lineTo(p2x, p2y)
                ctx.lineTo(p3x, p3y)
                ctx.lineWidth = marker.borderWidth
                ctx.strokeStyle = circle.border.color
                ctx.stroke()
            }
        }

        /*
          inner circle
        */
        Rectangle {
            width: circle.width/3
            height: width
            radius: .5 * width
            color: Style.colorMarkerWhite
            anchors.centerIn: circle
        }
    }
}
