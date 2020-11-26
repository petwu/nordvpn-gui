import QtQuick 2.12

import '.'

IconBase {
    Canvas {
        width: parent.size
        height: width
        anchors.centerIn: parent
        onPaint: {
            const ctx = getContext('2d')
            const a = width
            const r = width/2
            const l = parent.lineWidth

            // circle
            ctx.arc(r, r, r-l, 0, 2*Math.PI)

            // clock hands
            ctx.moveTo(r, r)
            ctx.lineTo(r, r - .75*(r-l))
            ctx.moveTo(r, r)
            ctx.lineTo(r + .5*(r-l), r)

            ctx.lineWidth = l
            ctx.strokeStyle = parent.color
            ctx.stroke()
        }
    }
}
