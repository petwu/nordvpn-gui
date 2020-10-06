import QtQuick 2.12
import QtQuick.Layouts 1.12

import Style 1.0

Column {
    id: collapsible
    width: parent.width
    height: expanded
            ? childrenRect.height
            : header.height
    clip: true
    spacing: 0

    property string title: ''
    property color titleColor: Style.colorCollapsibleTitle
    property color titleColorExpanded: Style.colorCollapsibleTitleExpanded
    property int handleSize: 22
    property color handleColor: Style.colorCollapsibleHandle
    property int headerPadding: 8
    property int animationDuration: 100
    property bool expanded: true

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

    Behavior on height {
        NumberAnimation {
            id: heightAnimation
            duration: 0
            easing.type: Easing.InOutQuad
        }
    }

    Column {
        width: parent.width

        Item {
            id: header
            width: parent.width
            height: title.height + 2*collapsible.headerPadding
            anchors.margins: collapsible.headerPadding

            RowLayout {
                x: collapsible.headerPadding
                y: collapsible.headerPadding
                spacing: Math.min(Math.max(8, collapsible.headerPadding), 16)

                Item {
                    width: collapsible.handleSize
                    height: width

                    Canvas {
                        rotation: collapsible.expanded
                                  ? 90
                                  : 0
                        width: height/2
                        height: parent.height
                        anchors.horizontalCenter: parent.horizontalCenter
                        onPaint: {
                            const ctx = getContext('2d')
                            const w = width
                            const h = height
                            const s = 2
                            // draw a triange width padding (vertical: s, horizontal: 2s) ...
                            ctx.beginPath()
                            // ... from top left ...
                            ctx.moveTo(s, 2*s)
                            // ... to right center ...
                            ctx.lineTo(w-s, h/2)
                            // ... to bottom left ...
                            ctx.lineTo(s, h-2*s)
                            // ... with the left side open
                            ctx.lineWidth = s
                            ctx.strokeStyle = collapsible.handleColor
                            ctx.stroke()
                        }

                        Behavior on rotation {
                            NumberAnimation {
                                duration: collapsible.animationDuration
                                easing.type: Easing.InOutQuad
                            }
                        }
                    }
                }

                Text {
                    id: title
                    text: collapsible.title
                    font.bold: true
                    color: collapsible.expanded
                           ? collapsible.titleColorExpanded
                           : collapsible.titleColor
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    heightAnimation.duration = collapsible.animationDuration
                    collapsible.expanded = !collapsible.expanded
                    delay(collapsible.animationDuration, () => {
                              heightAnimation.duration = 0
                          })
                }
            }
        }
    }
}
