import QtQuick 2.12
import QtQuick.Controls 2.12

import Style 1.0

import '.'
import '../general'

Item {
    width: parent.width
    height: parent.height
    clip: true

    /*! size boundaries of the map image */
    property int mapMinHeight: 640
    property int mapMaxHeight: 4800
    /*! zoom by scaling the map image with the given factor */
    property double zoomFactor: 1.3
    /*! size of the border/margin around the map */
    property int mapBorder: 128

    // keep center of map centered on size changes by shifting the x/y Positioner
    // by half of the width/height change
    onWidthChanged: {
        if (_.prevWidth === -1) {
            _.prevWidth = width
            return
        }
        map.x += (width - _.prevWidth)/2
        _.prevWidth = width
    }
    onHeightChanged: {
        if (_.prevHeight === -1){
            _.prevHeight = height
            return
        }
        map.y += (height - _.prevHeight)/2
        _.prevHeight = height
    }

    // private properties
    QtObject {
        id: _
        // flag that keeps the map centered until the first click/drag
        property bool mapCentered: true
        // list of all countries with their marker positions
        property var countryList: ConnectionMediator.countryList
        property double prevWidth: -1
        property double prevHeight: -1
    }

    Connections {
        target: ConnectionMediator
        onConnectingCountryIdChanged: (id) => (id !== -1) ? focusCountry(id) : null
        onConnectedCountryIdChanged: (id) => (id !== -1) ? focusCountry(id) : null
    }

    Component.onCompleted: {
        delay(1000, () => {
                  if (ConnectionMediator.isConnecting) {
                      focusCountry(ConnectionMediator.connectingCountryId)
                  } else if (ConnectionMediator.isConnected) {
                      focusCountry(ConnectionMediator.connectedCountryId)
                  }
              })
    }

    /*!
      delay waits the given delayTime in millisecons an the executes the callback
    */
    function delay(delayTime, callback) {
        function Timer() {
            return Qt.createQmlObject('import QtQuick 2.0; Timer {}', parent);
        }
        let timer = new Timer()
        timer.interval = delayTime
        timer.repeat = false
        timer.triggered.connect(callback)
        timer.start()
    }

    /*!
      focusCountry brings the country with the given id into focus, meaning it centers
      the country's marker. The map boundaries are respecting, which may cause the focused
      marker to be not centered but just as close to the center as possible within the given
      map boundaries.
    */
    function focusCountry(id) {
        _.mapCentered = false
        let markerOffsetLeft = 0
        let markerOffsetTop = 0
        // find country to get marker offset
        for (let i = 0; i < _.countryList.length; i++) {
            const c = _.countryList[i]
            if (c.id === id) {
                markerOffsetLeft = c.offsetLeft
                markerOffsetTop = c.offsetTop
                break
            }
        }
        // calculate needed shift to center marker
        const shiftX = -(map.width*markerOffsetLeft + map.x - mapArea.width/2)
        const shiftY = -(map.height*markerOffsetTop + map.y - mapArea.height/2)
        // backup animation durations
        const xad = mapXAnimation.duration
        const yad = mapYAnimation.duration
        // animate the X/Y shift within 200ms
        mapXAnimation.duration = 200
        mapYAnimation.duration = 200
        // shift by respecting the boundaries
        if (shiftX > 0) {
            map.x = Math.min(map.x + shiftX /* = desired x position */,
                             mapBorder /* = max. x position*/)
        } else {
            map.x = Math.max(map.x + shiftX /* = desired x position */,
                             -(map.width + mapBorder - mapArea.width) /* = min. x position */)
        }
        if (shiftY > 0) {
            map.y = Math.min(map.y + shiftY /* = desired y position */,
                             mapBorder /* = max. y position */)
        } else {
            map.y = Math.max(map.y + shiftY /* = desired y position */,
                             -(map.height + mapBorder - mapArea.height) /* = min. y position */)
        }
        // restore previous animation durations after the animaton is done
        delay(200, () => {
                  mapXAnimation.duration = xad
                  mapYAnimation.duration = yad
              })
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
        const curHeight = map.height
        const curWidth = map.width
        const curLeft = map.x
        const curRight = mapArea.width - (map.x + map.width)
        const curTop = map.y
        const curBottom = mapArea.height - (map.y + map.height)
        const ratio = map.width/map.height
        //
        let newHeight = curHeight
        if (zoomIn && curHeight*zoomFactor <= mapMaxHeight) {
            newHeight *= zoomFactor
        } else if (zoomOut && curHeight/zoomFactor >= mapMinHeight) {
            newHeight /= zoomFactor
        }
        let newWidth = newHeight * ratio
        let deltaWidth = newWidth - curWidth
        let deltaHeight = newHeight - curHeight
        let shiftX = -deltaWidth * (mouseX - curLeft)/(mapArea.width - curLeft - curRight)
        let shiftY = -deltaHeight * (mouseY - curTop)/(mapArea.height - curTop - curBottom)
        let newLeft = curLeft + shiftX
        let newRight = curRight - (deltaWidth + shiftX)
        let newTop = curTop + shiftY
        let newBottom = curBottom - (deltaHeight + shiftY)
        while (newLeft + newRight - 2*mapBorder > 0 || newTop + newBottom - 2*mapBorder > 0) {
            // => too small => scale up
            newHeight++
            newWidth = newHeight * ratio
            deltaWidth = newWidth - curWidth
            deltaHeight = newHeight - curHeight
            shiftX = -deltaWidth * (mouseX - curLeft)/(mapArea.width - curLeft - curRight)
            shiftY = -deltaHeight * (mouseY - curTop)/(mapArea.height - curTop - curBottom)
            newLeft = curLeft + shiftX
            newRight = curRight - (deltaWidth + shiftX)
            newTop = curTop + shiftY
            newBottom = curBottom - (deltaHeight + shiftY)
        }
        newLeft -= mapBorder
        newRight -= mapBorder
        newTop -= mapBorder
        newBottom -= mapBorder
        if (newLeft > 0) shiftX -= newLeft
        if (newRight > 0) shiftX += newRight
        if (newTop > 0) shiftY -= newTop
        if (newBottom > 0) shiftY += newBottom
        map.height = newHeight
        map.x += shiftX
        map.y += shiftY
    }

    /*!
      simplified zoom function that does not zoom towards the mouse pointer
      but rather towards the center of mapArea
    */
    function zoomCenter(zoomIn) {
        zoom(zoomIn,
             mapArea.width/2,
             mapArea.height/2)
    }

    /*!
      checkBounds ensures that the map stays inside mapArea
    */
    function checkBounds() {
        // fit map inside mapArea
        let deltaW = mapArea.width - map.width - 2*mapBorder
        let deltaH = mapArea.height - map.height - 2*mapBorder
        const ratio = map.width/map.height
        if (deltaW > 0 || deltaH > 0) {
            if (deltaH*ratio < deltaW) {
                // if: overcoming the vertical distance requires less scaling
                // then: add the missing height to map to fit the width of mapArea
                //       (the missing height can be defered from the missing width by considering the map ratio)
                map.height += deltaW/ratio
            } else {
                // else: overcoming the horizontal distance requires less scaling
                // then: add the missing height to map to fit the height of mapArea
                map.height += deltaH
            }
        }
        // keep map inside mapArea
        const left = map.x - mapBorder
        const right = mapArea.width - (map.x + map.width) - mapBorder
        const top = map.y - mapBorder
        const bottom = mapArea.height - (map.y + map.height) - mapBorder
        if (left > 0) map.x -= left
        if (right > 0) map.x += right
        if (top > 0) map.y -= top
        if (bottom > 0) map.y += bottom
    }

    /*
      background/ocean color
    */
    Rectangle {
        anchors.fill: parent
        color: Style.colorMapWater
    }

    /*
      the map image
    */
    Item {
        id: map
        width: mapImg.width
        height: 800
        anchors.centerIn: _.mapCentered ? parent : undefined

        Image {
            id: mapImg
            source: "image://map/" + Style.colorMapLand
            sourceSize: Qt.size(0, mapMaxHeight)
            fillMode: Image.PreserveAspectFit
            x: 0
            y: 0
            height: parent.height
        }

        Behavior on x {
            NumberAnimation {
                id: mapXAnimation
                duration: 0
                easing.type: Easing.InOutQuad
            }
        }

        Behavior on y {
            NumberAnimation {
                id: mapYAnimation
                duration: 0
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
        onPressed: () => _.mapCentered = false
        onReleased: checkBounds()
        onWidthChanged: checkBounds()
        onHeightChanged: checkBounds()
        onWheel: (wheel) => {
                     _.mapCentered = false
                     zoom(wheel.angleDelta.y > 0,
                          wheel.x,
                          wheel.y)
                 }
    }

    /*
      zoom controls
    */
    Button {
        id: zoomInBtn
        text: /* no qsTr() */ '＋'
        z: 100
        width: height
        anchors.right: parent.right
        anchors.bottom: zoomOutBtn.top
        anchors.rightMargin: Style.marginMapButtons
        anchors.bottomMargin: Style.marginMapButtons
        onClicked: zoomCenter(true)
    }

    Button {
        id: zoomOutBtn
        text: /* no qsTr() */ '－'
        z: 100
        width: height
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.rightMargin: Style.marginMapButtons
        anchors.bottomMargin: Style.marginMapButtons
        onClicked: zoomCenter(false)
    }

    /*
      markers
    */
    MouseArea {
        visible: DevMediator.showDebugInformation
        anchors.fill: map
        onClicked: {
            const l = (mouseX / map.width).toFixed(4)
            const t = (mouseY / map.height).toFixed(4)
            debugMarkerPosHint.text = `left: ${l}` + '\n' + `top:  ${t}`
        }
    }

    Text {
        id: debugMarkerPosHint
        visible: DevMediator.showDebugInformation
        anchors.left: mapArea.left
        anchors.leftMargin: 8
        anchors.bottom: mapArea.bottom
        anchors.bottomMargin: 48
        text: '<i>click to<br/>show coords</i>'
    }

    Repeater {
        model: _.countryList

        Marker {
            countryId: modelData.id
            name: modelData.name
            anchors.left: map.left
            anchors.top: map.top
            anchors.leftMargin: map.width * modelData.offsetLeft
            anchors.topMargin: map.height * modelData.offsetTop
        }
    }
}
