import QtQuick 2.12

import Style 1.0

Item {
    id: ratingStars
    height: 28
    width: 5*height

    property color color: Style.colorStatusPanelStarsOutline
    property double strokeWidth: height/4
    property int rating: 0

    signal rated

    Row {
        Repeater {
            model: 5

            Canvas {
                width: height
                height: ratingStars.height
                onPaint: {
                    const ctx = getContext("2d")

                    // side length
                    const a = height
                    // angle between two points of the star
                    const alpha = 72 * Math.PI/180
                    // outer radius
                    const r_o = a/2 / Math.sin(alpha)
                    // inner radius
                    const r_i = (r_o * Math.cos(alpha)) / Math.cos(alpha/2)

                    const points = []
                    let i = 0
                    // calculate outer points
                    for (let alpha_i = 0; alpha_i < 2*Math.PI; alpha_i += alpha) {
                        const x = r_o * Math.sin(alpha_i) + a/2
                        const y = a/2 - r_o * Math.cos(alpha_i)
                        points[i] = { x, y }
                        i += 2
                    }
                    i = 1
                    // calculate inner points
                    for (let alpha_j = alpha/2; alpha_j < 2*Math.PI; alpha_j += alpha) {
                        const x = r_i * Math.sin(alpha_j) + a/2
                        const y = a/2 - r_i * Math.cos(alpha_j)
                        points[i] = { x, y }
                        i += 2
                    }

                    // draw star
                    ctx.beginPath()
                    ctx.moveTo(points[0].x, points[0].y)
                    for (i = 1; i < points.length; i++) {
                        ctx.lineTo(points[i].x, points[i].y)
                    }
                    ctx.closePath()
                    ctx.fillStyle = ratingStars.color
                    ctx.fill()
                }

                Canvas {
                    visible: ratingStars.rating <= modelData
                    anchors.centerIn: parent
                    width: height
                    height: parent.height - 2*ratingStars.strokeWidth
                    onPaint: {
                        /*
                            => same as above canvas, but smaller (see const a) and in white
                        */
                        const ctx = getContext("2d")
                        const a = height
                        const alpha = 72 * Math.PI/180
                        const r_o = a/2 / Math.sin(alpha)
                        const r_i = (r_o * Math.cos(alpha)) / Math.cos(alpha/2)
                        const points = []
                        let i = 0
                        for (let alpha_i = 0; alpha_i < 2*Math.PI; alpha_i += alpha) {
                            const x = r_o * Math.sin(alpha_i) + a/2
                            const y = a/2 - r_o * Math.cos(alpha_i)
                            points[i] = { x, y }
                            i += 2
                        }
                        i = 1
                        for (let alpha_j = alpha/2; alpha_j < 2*Math.PI; alpha_j += alpha) {
                            const x = r_i * Math.sin(alpha_j) + a/2
                            const y = a/2 - r_i * Math.cos(alpha_j)
                            points[i] = { x, y }
                            i += 2
                        }
                        ctx.beginPath()
                        ctx.moveTo(points[0].x, points[0].y)
                        for (i = 1; i < points.length; i++) {
                            ctx.lineTo(points[i].x, points[i].y)
                        }
                        ctx.closePath()
                        ctx.fillStyle = Style.colorStatusPanelBkgd
                        ctx.fill()
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    hoverEnabled: true
                    onEntered: ratingStars.rating = modelData + 1
                    onExited: ratingStars.rating = 0
                    onClicked: {
                        MapMediator.rate(ratingStars.rating)
                        rated()
                    }
                }
            }
        }
    }
}
