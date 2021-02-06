pragma Singleton

import QtQuick 2.12

Item {
    SystemPalette {
        id: systemTheme
        colorGroup: SystemPalette.Active
        onPaletteChanged: determineTheme()
    }
    SystemPalette {
        id: systemThemeDisabled
        colorGroup: SystemPalette.Disabled
    }

    Component.onCompleted: determineTheme()

    /*!
     * Populate whether to use dark or light colors.
     * The QML color type (https://doc.qt.io/qt-5/qml-color.html) has r, g, b, a properties
     * value ranges from 0 to 1. A middle value of 0.5 is therefore equally light and dark.
     * We use this to determine, whether the system palette is more of a light or a dark
     * theme, based on the base color and according to this formula:
     * isDark := (r+g+b)/3 < 0.5    with    r,g,b ∊ { x | 0≤x≤1 ∧ x∊ℤ}
     */
    function determineTheme() {
        const r = systemTheme.base.r
        const g = systemTheme.base.g
        const b = systemTheme.base.r
        isDarkTheme = (r+g+b)/3 < 0.5
    }

    // system color theme: dark or light?
    property bool isDarkTheme

    // system colors
    readonly property color colorBase: systemTheme.base
    readonly property color colorBaseAlternate: systemTheme.alternateBase
    readonly property color colorShadow: systemTheme.shadow
    readonly property color colorText: systemTheme.text
    readonly property color colorMid: systemTheme.mid
    readonly property color colorLight: systemTheme.light
    readonly property color colorButton: systemTheme.button
    readonly property color colorButtonText: systemTheme.buttonText
    readonly property color colorHighlight: systemTheme.highlight
    readonly property color colorHighlightText: systemTheme.highlightedText

    // system colors (disabled)
    readonly property color colorTextDisabled: systemThemeDisabled.text

    // base colors
    readonly property color colorPrimary: '#1e6291'
    readonly property color colorPrimaryAlternate: '#5a89aa'
    readonly property color colorGreen: '#75b81b'
    readonly property color colorRed: '#dc5152'
    readonly property color colorOrange: '#f18129'
    readonly property color colorYellow: '#ffc805'
    readonly property color colorTextAlternate: isDarkTheme ? Qt.darker(colorText, 1.5) : Qt.lighter(colorText, 1.5)

    // semantic colors
    readonly property color colorError: colorRed
    readonly property color colorSuccess: colorGreen

    // map
    readonly property color colorMarkerDefault: colorPrimary
    readonly property color colorMarkerInactive: colorPrimaryAlternate
    readonly property color colorMarkerConnected: colorGreen
    readonly property color colorMarkerSecondary: colorBaseAlternate
    readonly property color colorMapLand: colorBaseAlternate
    readonly property color colorMapWater: isDarkTheme ? '#4a7997' : '#b8cedc'

    // status panel
    readonly property color colorStatusPanelBkgd: colorBase
    readonly property color colorStatusPanelShadow: colorShadow
    readonly property color colorStatusPanelDisconnected: colorRed
    readonly property color colorStatusPanelConnecting: colorOrange
    readonly property color colorStatusPanelConnected: colorGreen
    readonly property color colorStatusPanelSeconary: colorTextAlternate
    readonly property color colorStatusPanelStarsOutline: colorYellow

    // sidebar
    readonly property color colorCollapsibleTitle: isDarkTheme ? Qt.darker(colorText, 1.25) : Qt.lighter(colorText, 1.25)
    readonly property color colorCollapsibleTitleExpanded: colorText
    readonly property color colorCollapsibleHandle: colorIcon

    // canvas icons (derived from IconBase)
    readonly property color colorIcon: colorTextAlternate

    // spacing
    readonly property int marginMapButtons: 4

    // sidebar popups
    readonly property int sidebarPopupWidth: 320

    // font related
    readonly property int fontSizeSmall: 0.8 * Qt.application.font.pixelSize
    readonly property int fontSizeLarge: 1.3 *Qt.application.font.pixelSize
}
