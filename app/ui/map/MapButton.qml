import QtQuick 2.12

import Style 1.0

import '../general'

Button {
    z: 1000
    anchors.topMargin: Style.marginMapButtons
    anchors.leftMargin: Style.marginMapButtons
    anchors.rightMargin: Style.marginMapButtons
    anchors.bottomMargin: Style.marginMapButtons
    width: 32
    height: width
    font.bold: true
    padding: 2
    font.pixelSize: 1.5*Qt.application.font.pixelSize
    foregroundColor: Style.colorMapButtonFrgd
    backgroundColor: Style.colorMapButtonBkgd
}
