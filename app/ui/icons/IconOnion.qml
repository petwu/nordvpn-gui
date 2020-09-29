import QtQuick 2.12

import '.'

IconBase {
    Canvas {
        width: parent.contentWidth
        height: width
        anchors.centerIn: parent
        onPaint: {
            const ctx = getContext('2d')
            const a = width
            const l = parent.lineWidth

            // outline
            ctx.moveTo(.62*a, l)
            ctx.lineTo(.5*a, .07*a+l)
            ctx.bezierCurveTo(-.1*a, .45*a, 0, .95*a-2*l, .5*a, a-2*l)
            ctx.bezierCurveTo(a, .95*a-2*l, 1.1*a, .45*a, .5*a, .07*a+l)
            ctx.lineTo(.38*a, l)

            // roots
            ctx.moveTo(.45*a, a-2*l)
            ctx.lineTo(.55*a, a-2*l)
            ctx.lineTo(.55*a, a-l)
            ctx.lineTo(.45*a, a-l)

            // scale leaf
            ctx.moveTo(.43*a, .3*a)
            ctx.quadraticCurveTo(.34*a, .37*a, .3*a, .45*a)

            ctx.lineWidth = l
            ctx.strokeStyle = parent.color
            ctx.stroke()
        }
    }
}
