import QtQuick 2.12

import '.'

IconBase {
    Canvas {
        width: parent.size
        height: width
        anchors.centerIn: parent
        onPaint: {
            const ctx = getContext('2d')
            const l = parent.lineWidth
            const a = width
            const r1 = width*9/25 - l
            const r2 = width*5/25 - l
            const pi = Math.PI

            const closeUp = (ctx) => {
                ctx.lineWidth = l
                ctx.strokeStyle = parent.color
                ctx.stroke()
            }

            // 3 little circles
            const x1 = a/2 - r1*Math.cos(pi/6)
            const x2 = a/2
            const x3 = a - x1
            const y1 = a/2 - r1
            const y2 = a/2 + r1*Math.sin(pi/6)
            // top center
            ctx.beginPath()
            ctx.arc(x2, y1, r2, 0, 2*pi)
            closeUp(ctx)
            // bottom left
            ctx.beginPath()
            ctx.arc(x1, y2, r2, 0, 2*pi)
            closeUp(ctx)
            // bottom right
            ctx.beginPath()
            ctx.arc(x3, y2, r2, 0, 2*pi)
            closeUp(ctx)

            // 1 big circle out of 3 little arcs
            const phiOffset = r2/r1
            // top right arc
            ctx.beginPath()
            ctx.arc(a/2, a/2, r1, 1/6*pi-phiOffset, -(pi/2-phiOffset), true)
            closeUp(ctx)
            // top left arc
            ctx.beginPath()
            ctx.arc(a/2, a/2, r1, -(pi/2+phiOffset), -(7/6*pi-phiOffset), true)
            closeUp(ctx)
            // bottom center arc
            ctx.beginPath()
            ctx.arc(a/2, a/2, r1, -(7/6*pi+phiOffset), (1/6*pi+phiOffset), true)
            closeUp(ctx)
        }
    }
}
