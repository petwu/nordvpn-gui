import QtQuick 2.12

Item {
    width: parent.width
    height: parent.height

    // size boundaries of the map image
    property int mapMinHeight: 400
    property int mapMaxHeight: 4000
    // zoom by scaling the map image with the given factor
    property double zoomFactor: 1.3
    // operate in a grid rather than pixel by pixel
    property int gridSize: 5
    // size of the border/margin around the map
    property int maxBorder: 50
    // flag that keeps the map centered until the first click/drag
    property bool firstInteraction: true
    // transition duration
    property int behaviorDuration: 100
    property bool checkBoundsAfterZoomIsQueued: false

    /*!
      g(n) rounds n to the next multiplicative of gridSize, e.g.
      - 123 => 125
      - 112 => 110
      - 2.5 => 5
    */
    function g(n) {
        if ((n % gridSize) > gridSize/2) {
            return Math.ceil(n/gridSize) * gridSize
        }
        return Math.floor(n/gridSize) * gridSize
    }

    /*!
      delay waits the given delayTime in millisecons an the executes the callback
    */
    function delay(delayTime, callback) {
        function Timer() {
            return Qt.createQmlObject("import QtQuick 2.0; Timer {}", parent);
        }
        let timer = new Timer()
        timer.interval = delayTime
        timer.repeat = false
        timer.triggered.connect(callback)
        timer.start()
    }

    /*!
      zooms the map

      parameters:
      - zoomIn [bool]       true to zoom in, false to zoom out
      - mouseX [int]        x coordinate of the mouse inside mapArea during zoom
      - mouseY [int]        y coordinate of the mouse inside mapArea during zoom
    */
    function zoom(zoomIn, mouseX, mouseY) {
        const zoomOut = !zoomIn
        // check if bounds are reached => no further zooming out
        if (zoomOut && (mapArea.width - (map.width + 2*maxBorder) >= 0 || mapArea.height - (map.height + 2*maxBorder) >= 0)) {
            checkBounds()
            return
        }

        // get new height
        let h = map.height
        let w = map.width
        if (zoomIn && g(h*zoomFactor) <= mapMaxHeight) {
            h *= zoomFactor
        } else if (zoomOut && g(h/zoomFactor) >= mapMinHeight) {
            h /= zoomFactor
        }
        h = g(h)
        const deltaH = Math.abs(map.height - h)
        const deltaW = w * Math.abs(1 - h / map.height)
        // correct (x,y) of map depending on the mouse position in order to zoom the area
        // beneath the mouse and no the currently centered area
        let xShift = 0
        let yShift = 0
        if (Math.abs(g(mouseX) - g(mapArea.width/2)) <= 4*gridSize && Math.abs(g(mouseY) - g(mapArea.height/2)) <= 4*gridSize) {
            // mouse is in center => no correction required
        } else if (deltaH === 0) {
            // no zooming => no correction required
        } else {
            const offsetX = g(mouseX - mapArea.width/2)
            const offsetY = g(mouseY - mapArea.height/2)

            xShift = deltaW * mouseX / mapArea.width
            yShift = deltaH * mouseY / mapArea.height
            if (zoomIn) {
                xShift *= -1
                yShift *= -1
            }
        }
        // apply zoom
        map.height = h
        map.x += xShift
        map.y += yShift
        delay(behaviorDuration, () => checkBounds())
    }

    /*!
      checkBounds ensures that the map stays inside mapArea
    */
    function checkBounds(secondTime = false) {
        // fit map inside mapArea
        let width = map.width + 2*maxBorder
        let height = map.height + 2*maxBorder
        let deltaW = mapArea.width - width
        let deltaH = mapArea.height - height
        const ratio = width/height
        if (deltaW > 0 && deltaH > 0) {
            if (deltaH*ratio < deltaW) {
                // if: overcoming the vertical distance requires less scaling
                // then: add the missing height to map to fit the height of mapArea
                map.height += deltaH
                deltaH = 0
            } else {
                // else: overcoming the horizontal distance requires less scaling
                // then: add the missing height to map to fit the width of mapArea
                //       (the missing height can be defered from the missing width by considering the map ratio)
                map.height += deltaW/ratio
                deltaW = 0
            }
        }
        // keep map+maxBorder inside mapArea
        const left = map.x - maxBorder
        const right = mapArea.width - (map.x + map.width + maxBorder)
        const top = map.y - maxBorder
        const bottom = mapArea.height - (map.y + map.height + maxBorder)
        if (deltaW === 0) {
            // if: map fits the width of mapArea
            // then: center map vertically
            map.x = maxBorder
            map.y -= (top - bottom)/2
        } else if (deltaH === 0) {
            // if: map fits the height of mapArea
            // then: center map horizontally
            map.x -= (left - right)/2
            map.y = maxBorder
        } else {
            // else: map convers mapArea completely
            // then: ensure all boundaries are adhered
            if (left > 0) map.x -= left
            if (right > 0) map.x += right
            if (top > 0) map.y -= top
            if (bottom > 0) map.y += bottom
        }
        if (!secondTime) {
            delay(behaviorDuration, () => checkBounds(true))
        }
    }

    /*
      the map image
    */
    Image {
        id: map
        source: "qrc:/img/map.svg"
        sourceSize: Qt.size(0, mapMaxHeight)
        fillMode: Image.PreserveAspectFit
        height: 720
        anchors.centerIn: firstInteraction ? parent : undefined

        Behavior on x {
            NumberAnimation {
                duration: behaviorDuration
                easing.type: Easing.InOutQuad
            }
        }

        Behavior on y {
            NumberAnimation {
                duration: behaviorDuration
                easing.type: Easing.InOutQuad
            }
        }

        Behavior on width {
            NumberAnimation {
                duration: behaviorDuration
                easing.type: Easing.InOutQuad
            }
        }

        Behavior on height {
            NumberAnimation {
                duration: behaviorDuration
                easing.type: Easing.InOutQuad
            }
        }
    }

    /*
      the MouseArea that handles zooming and panning of the map image
    */
    MouseArea {
        id: mapArea
        width: parent.width
        height: parent.height

        drag.target: map
        onPressed: () => firstInteraction = false
        onReleased: checkBounds()
        onWidthChanged: checkBounds()
        onHeightChanged: checkBounds()
        onWheel: (wheel) => {
                     firstInteraction = false
                     zoom(wheel.angleDelta.y > 0,
                          wheel.x,
                          wheel.y)
                 }
    }
}
