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
            const pi = Math.PI

            const closeUp = (ctx) => {
                ctx.lineWidth = l
                ctx.strokeStyle = parent.color
                ctx.stroke()
            }

            // big lock
            let r = .3*a - l
            let x1 = l
            let x2 = .425*a - r
            let x3 = .425*a + r
            let x4 = .525*a
            let x5 = .85*a - l/2
            let y1 = r + l/2
            let y2 = r + .175*a
            let y3 = a - 1.5*l
            ctx.beginPath()
            ctx.moveTo(x3, y2)
            ctx.arc((x2+x3)/2, y1, r, 0, pi, true)
            ctx.lineTo(x2, y2)
            closeUp(ctx)
            ctx.beginPath()
            ctx.moveTo(x5, y2)
            ctx.lineTo(x1, y2)
            ctx.lineTo(x1, y3)
            ctx.lineTo(x4, y3)
            closeUp(ctx)

            // small lock
            r /= 2.375
            x1 = x4 + 1.5*l
            x2 = .775*a - r
            x3 = .775*a + r
            x4 = a - l
            y1 = .625*a
            y2 = .725*a
            y3 = a - l/2
            ctx.beginPath()
            ctx.moveTo(x3, y2)
            ctx.arc((x2+x3)/2, y1, r, 0, pi, true)
            ctx.lineTo(x2, y2)
            closeUp(ctx)
            ctx.beginPath()
            ctx.moveTo(x4, y2)
            ctx.lineTo(x1, y2)
            ctx.lineTo(x1, y3)
            ctx.lineTo(x4, y3)
            ctx.lineTo(x4, y2)
            closeUp(ctx)
        }
    }
}
