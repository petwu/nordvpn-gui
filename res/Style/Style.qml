pragma Singleton

import QtQuick 2.12

Item {
    SystemPalette {
        id: systemTheme
        colorGroup: SystemPalette.Active
    }

    // system colors
    readonly property color colorBase: systemTheme.base
    readonly property color colorBaseAlternate: systemTheme.alternateBase
    readonly property color colorShadow: systemTheme.shadow
    readonly property color colorText: systemTheme.text
    readonly property color colorMid: systemTheme.mid
    readonly property color colorButton: systemTheme.button
    readonly property color colorButtonText: systemTheme.buttonText

    // base colors
    readonly property color colorPrimary: '#1e6291'
    readonly property color colorPrimaryAlternate: '#5a89aa'
    readonly property color colorGreen: '#75b81b'
    readonly property color colorRed: '#dc5152'
    readonly property color colorOrange: '#f18129'
    readonly property color colorYellow: '#ffc805'

    // map
    readonly property color colorMarkerDefault: colorPrimary
    readonly property color colorMarkerInactive: colorPrimaryAlternate
    readonly property color colorMarkerConnected: colorGreen
    readonly property color colorMarkerSecondary: colorBaseAlternate
    readonly property color colorMapLand: colorBaseAlternate
    readonly property color colorMapWater: IsDarkTheme ? '#4a7997' : '#b8cedc'

    // status panel
    readonly property color colorStatusPanelBkgd: colorBase
    readonly property color colorStatusPanelShadow: colorShadow
    readonly property color colorStatusPanelDisconnected: colorRed
    readonly property color colorStatusPanelConnecting: colorOrange
    readonly property color colorStatusPanelConnected: colorGreen
    readonly property color colorStatusPanelSeconary: IsDarkTheme ? Qt.darker(colorText, 1.5) : Qt.ligher(colorText, 1.5)
    readonly property color colorStatusPanelStarsOutline: colorYellow

    // sidebar
    readonly property color colorCollapsibleTitle: IsDarkTheme ? Qt.darker(colorText, 1.25) : Qt.ligher(colorText, 1.25)
    readonly property color colorCollapsibleTitleExpanded: colorText
    readonly property color colorCollapsibleHandle: colorIcon

    // canvas icons (derived from IconBase)
    readonly property color colorIcon: IsDarkTheme ? Qt.darker(colorText, 1.5) : Qt.ligher(colorText, 1.5)

    // spacing
    readonly property int marginMapButtons: 4
}
