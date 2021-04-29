import QtQuick 2.12

import Style 1.0

Item {
    id: marker
    width: 0
    height: 0
    z: _.disconnected ? 1 : ConnectionMediator.countryList.length+1

    property int countryId: -1
    property string name: ''
    property double scaleFactor: 1
    property double scaleDiff: 0.35
    property double pointerHeight: .25
    property int radius: 18
    property int borderWidth: (radius*scaleFactor)/8

    QtObject {
        id: _
        /*!
         * actual scale factor that is applied to the marker, whereas
         * marker.scaleFactor is the initial scale factor
         */
        property double scaleFactor: marker.scaleFactor
        /*! properties that describe the connection status */
        property bool disconnected: ConnectionMediator.connectedCountryId !== marker.countryId &&
                                    ConnectionMediator.connectingCountryId !== marker.countryId
        property bool connecting: ConnectionMediator.connectingCountryId === marker.countryId
        property bool connected: ConnectionMediator.connectedCountryId === marker.countryId
        /*! the primary color (= highlight color) that changes depending on the connection status */
        property color colorPrimary: connected
                                     /* if connected => highlight */
                                     ? Style.colorMarkerConnected
                                     : (ConnectionMediator.connectingCountryId === -1
                                        /* else if idle/nothing connected or connecting => use default color for all markers */
                                        ? Style.colorMarkerDefault
                                        : (connecting
                                           /* else if connecting => use default color only for the connecting marker ... */
                                           ? Style.colorMarkerDefault
                                           /* ... and another color for all other markers */
                                           : Style.colorMarkerInactive))
        /*! the background and border color to frame the marker */
        property color colorSecondary: Style.colorMarkerSecondary
        /*!
         * the color of the text containing the country name that is displayed below
         * the marker; uses either the background color or the general text color,
         * depending on the brightness of the primary color
         */
        property color colorText: {
            let r = colorPrimary.r
            let g = colorPrimary.g
            let b = colorPrimary.b
            return (Math.max(r+g+b) >= 0.5 && (r+g+b) > 1.2)
                ? Style.colorMarkerSecondary
                : Style.colorText
        }
        /*! indicator whether the marker is currently hover by a mouse */
        property bool hovered: false

        /*! update scale, z-index and text visibility on connection status changes */
        onConnectedChanged: setHighlight()
        onConnectingChanged: setHighlight()
        onDisconnectedChanged: setHighlight()
    }

    /*!
     * Update the highlight status of the marker:
     * - set scale factor increase/decrease size
     * - set z index to elevate over other markers
     * - set the text visibility
     *
     * parameters:
     * - hoverState: 0=unchanged (default), 1=mouse-enter, 2=mouse-exit
     */
    function setHighlight(hoverState = 0) {
        switch (hoverState) {
        case 0: /* unchanged */; break
        case 1: _.hovered = true; break
        case 2: _.hovered = false; break
        }
        let highlighted = _.connected || _.connecting || _.hovered
        _.scaleFactor = marker.scaleFactor + (highlighted ? marker.scaleDiff : 0)
        marker.z = highlighted ? ConnectionMediator.countryList.length+1 : 1
        markerText.visible = _.hovered
    }

    Connections {
        target: ConnectionMediator
        onConnectedCountryIdChanged: triangle.requestPaint()
        onConnectingCountryIdChanged: triangle.requestPaint()
    }

    Timer {
        interval: 500
        running: true
        repeat: true
        onTriggered: {
            // repaint triangle because of some strange color leak (= narrow blue line)
            // when switching color from blue (connecting) to green (connected)
            // => with this timer the leak fades after a few seconds
            // if the reader of this comments knows any better solutions
            // => you're welcome to implement them :D
            if (_.connected) {
                triangle.requestPaint()
            }
        }
    }

    MouseArea {
        id: markerArea
        width: 2*marker.radius*_.scaleFactor
        height: width * (1 + marker.pointerHeight)
        x: -width/2
        y: -height
        cursorShape: Qt.PointingHandCursor
        hoverEnabled: true
        onEntered: setHighlight(1)
        onExited: setHighlight(2)
        onClicked: {
            if (_.disconnected) {
                ConnectionMediator.connectToCountryById(marker.countryId)
            }
        }

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
            color: _.colorPrimary
            border.width: marker.borderWidth
            border.color: _.colorSecondary
        }

        /*
          triangle below outer circle giving the marker its typical shape
        */
        Canvas {
            id: triangle
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
            color: _.colorSecondary
            anchors.centerIn: circle
        }

        /*
          name tag
        */
        Rectangle {
            id: markerText
            visible: false
            width: _markerText.width + 24
            height: _markerText.height + 10
            anchors.top: markerArea.bottom
            x: -(width - markerArea.width)/2
            z: 10
            color: _.colorPrimary
            border.width: marker.borderWidth
            border.color: _.colorSecondary
            radius: 8

            Text {
                id: _markerText
                text: marker.name
                anchors.centerIn: parent
                color: _.colorText
            }
        }
    }
}
