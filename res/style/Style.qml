pragma Singleton

import QtQuick 2.0

QtObject {
    // base colors
    readonly property color colorWhite: '#f1f1f1'
    readonly property color colorLightGray: '#e7e8e9'
    readonly property color colorDarkGray: '#73757a'
    readonly property color colorBlueDark: '#1e6291'
    readonly property color colorBlueLight: '#5a89aa'
    readonly property color colorGreen: '#75b81b'
    readonly property color colorRed: '#dc5152'
    readonly property color colorOrange: '#f18129'
    readonly property color colorYellow: '#ffc805'

    // controls
    readonly property color colorButtonDefaultBkgd: colorBlueDark
    readonly property color colorButtonDefaultFrgd: colorLightGray
    readonly property color colorButtonDangerBkgd: colorRed
    readonly property color colorButtonDangerFrgd: colorLightGray

    // map
    readonly property color colorMarkerBlueDark: colorBlueDark
    readonly property color colorMarkerBlueLight: colorBlueLight
    readonly property color colorMarkerGreen: colorGreen
    readonly property color colorMarkerWhite: colorWhite
    readonly property color colorMapWater: '#b8cedc'

    // status panel
    readonly property color colorStatusPanelBkgd: colorWhite
    readonly property color colorStatusPanelShadow: colorDarkGray
    readonly property color colorStatusPanelDisconnected: colorRed
    readonly property color colorStatusPanelConnecting: colorOrange
    readonly property color colorStatusPanelConnected: colorGreen
    readonly property color colorStatusPanelSeconary: colorDarkGray
    readonly property color colorStatusPanelStarsOutline: colorYellow
}
